package main

import (
	"bufio"
	"bytes"
	"errors"
	"fmt"
	"log"
	"os"
	"os/exec"
	"strconv"
	"strings"
	"sync"
	"syscall"
	"time"

	debVersion "github.com/knqyf263/go-deb-version"
	lastore "github.com/linuxdeepin/go-dbus-factory/com.deepin.lastore"
	ofdbus "github.com/linuxdeepin/go-dbus-factory/org.freedesktop.dbus"
	dbus "pkg.deepin.io/lib/dbus1"
	"pkg.deepin.io/lib/dbusutil"
)

const (
	dbusBackendDebInterface = "com.deepin.AppStore.Backend.Deb"
	dbusJobPathPrefix       = dbusBackendPath + "/Job"
	dbusJobInterface        = dbusBackendDebInterface + ".Job"
)

func init() {
	log.SetFlags(log.Lshortfile)
}

// Backend for deb package
type Backend struct {
	metadata         *Metadata
	service          *dbusutil.Service
	sysSigLoop       *dbusutil.SignalLoop
	lastore          *lastore.Lastore
	dbusDaemon       *ofdbus.DBus
	lastoreJobList   []dbus.ObjectPath
	lastoreJobListMu sync.Mutex
	jobs             map[dbus.ObjectPath]*Job
	PropsMu          sync.RWMutex
	JobList          []dbus.ObjectPath
	methods          *struct {
		Install               func() `in:"localizedName,id" out:"job"`
		Remove                func() `in:"localizedName,id" out:"job"`
		ListInstalled         func() `out:"installedInfoList"`
		QueryVersion          func() `in:"idList" out:"versionInfoList"`
		QueryDownloadSize     func() `in:"id" out:"size"`
		QueryInstallationTime func() `in:"idList" out:"installationTimeList"`
		FixError              func() `in:"errType" out:"job"`
	}

	// TODO: move to all install
	block *blocklist
}

func newBackend(service *dbusutil.Service) (*Backend, error) {
	systemConn, err := dbus.SystemBus()
	if err != nil {
		return nil, err
	}
	lastoreObj := lastore.NewLastore(systemConn)
	dbusDaemon := ofdbus.NewDBus(systemConn)
	sysSigLoop := dbusutil.NewSignalLoop(systemConn, 50)
	return &Backend{
		service:    service,
		lastore:    lastoreObj,
		dbusDaemon: dbusDaemon,
		sysSigLoop: sysSigLoop,
		jobs:       make(map[dbus.ObjectPath]*Job),
	}, nil
}

func (b *Backend) updatePropJobList() {
	var jobList []dbus.ObjectPath
	for _, job := range b.jobs {
		jobList = append(jobList, job.getPath())
	}
	b.JobList = jobList
	err := b.service.EmitPropertyChanged(b, "JobList", jobList)
	if err != nil {
		log.Println("warning:", err)
	}
}

func (b *Backend) handleDaemonOnline() {
	log.Println("lastore-daemon online")
}

func (b *Backend) handleDaemonOffline() {
	log.Println("lastore-daemon offline")
	b.lastoreJobListMu.Lock()
	b.lastoreJobList = nil
	b.lastoreJobListMu.Unlock()

	b.PropsMu.Lock()
	for jobPath, job := range b.jobs {
		delete(b.jobs, jobPath)
		job.destroy()
		err := b.service.StopExport(job)
		if err != nil {
			log.Printf("failed to stop export job %s: %v", job.ID, err)
		}
	}

	b.JobList = []dbus.ObjectPath{}
	b.service.EmitPropertyChanged(b, "JobList", b.JobList)
	b.PropsMu.Unlock()
}

func (b *Backend) init() {
	b.sysSigLoop.Start()

	b.dbusDaemon.InitSignalExt(b.sysSigLoop, true)
	b.dbusDaemon.ConnectNameOwnerChanged(
		func(name string, oldOwner string, newOwner string) {
			if name == b.lastore.ServiceName_() {
				if newOwner == "" {
					b.handleDaemonOffline()
				} else {
					b.handleDaemonOnline()
				}
			}
		})

	b.lastore.InitSignalExt(b.sysSigLoop, true)
	b.lastore.JobList().ConnectChanged(func(hasValue bool, value []dbus.ObjectPath) {
		if !hasValue {
			return
		}

		log.Printf("lastore JobList changed %#v\n", value)

		b.lastoreJobListMu.Lock()
		var removedJobPaths []dbus.ObjectPath
		for _, jobPath := range b.lastoreJobList {
			if !objectPathSliceContains(value, jobPath) {
				removedJobPaths = append(removedJobPaths, jobPath)
			}
		}
		b.lastoreJobList = value
		b.lastoreJobListMu.Unlock()

		b.PropsMu.Lock()
		for _, jobPath := range removedJobPaths {
			job, ok := b.jobs[jobPath]
			if ok {
				delete(b.jobs, jobPath)
				b.updatePropJobList()

				log.Println("destroy job", job.core.Path_())
				job.destroy()
				b.service.StopExport(job)
			}
		}
		b.PropsMu.Unlock()
	})

	b.lastoreJobListMu.Lock()
	b.lastoreJobList, _ = b.lastore.JobList().Get(0)
	b.lastoreJobListMu.Unlock()
}

func objectPathSliceContains(slice []dbus.ObjectPath, path dbus.ObjectPath) bool {
	for _, v := range slice {
		if v == path {
			return true
		}
	}
	return false
}

// GetInterfaceName return dbus interface name
func (*Backend) GetInterfaceName() string {
	return dbusBackendDebInterface
}

func (b *Backend) addJob(jobPath dbus.ObjectPath) (dbus.ObjectPath, error) {
	log.Println("add job", jobPath)

	b.PropsMu.Lock()
	defer b.PropsMu.Unlock()
	job, ok := b.jobs[jobPath]
	if ok {
		log.Printf("job %s exist", job.ID)
		return job.getPath(), nil
	}

	job, err := newJob(b, jobPath)
	if err != nil {
		return "/", err
	}

	myJobPath := job.getPath()
	err = b.service.Export(myJobPath, job)
	if err != nil {
		log.Printf("warning: failed to export job %s: %v", job.ID, err)
		return "/", err
	}

	b.jobs[jobPath] = job
	b.updatePropJobList()
	return myJobPath, nil
}

// QueryDownloadSize return package download size
func (b *Backend) QueryDownloadSize(id string) (int64, *dbus.Error) {
	size, err := b.lastore.PackagesDownloadSize(0, []string{id})
	if err != nil {
		return 0, dbusutil.ToError(err)
	}
	return size, nil
}

// Install package and notify with localizedName when finish
func (b *Backend) Install(localizedName, id string) (dbus.ObjectPath, *dbus.Error) {
	b.service.DelayAutoQuit()
	log.Printf("install %q %q\n", localizedName, id)

	b.block.remove(id)

	jobPath, err := b.lastore.InstallPackage(0, localizedName, id)
	if err != nil {
		return "/", dbusutil.ToError(err)
	}

	myJobPath, err := b.addJob(jobPath)
	if err != nil {
		return "/", dbusutil.ToError(err)
	}
	return myJobPath, nil
}

// Remove package and notify with localizedName when finish
func (b *Backend) Remove(localizedName, id string) (dbus.ObjectPath, *dbus.Error) {
	b.service.DelayAutoQuit()
	log.Printf("remove %q %q\n", localizedName, id)

	b.block.add(id)

	jobPath, err := b.lastore.RemovePackage(0, localizedName, id)
	if err != nil {
		return "/", dbusutil.ToError(err)
	}

	myJobPath, err := b.addJob(jobPath)
	if err != nil {
		return "/", dbusutil.ToError(err)
	}
	return myJobPath, nil
}

// FixError will try to fix apt error
func (b *Backend) FixError(errType string) (dbus.ObjectPath, *dbus.Error) {
	b.service.DelayAutoQuit()
	log.Println("fixError", errType)
	jobPath, err := b.lastore.FixError(0, errType)
	if err != nil {
		return "/", dbusutil.ToError(err)
	}

	myJobPath, err := b.addJob(jobPath)
	if err != nil {
		return "/", dbusutil.ToError(err)
	}
	return myJobPath, nil
}

// ListInstalled will list all install package
func (b *Backend) ListInstalled() (result []PackageInstalledInfo, busErr *dbus.Error) {
	b.service.DelayAutoQuit()

	cmd := exec.Command("/usr/bin/dpkg-query", "--show", "-f",
		"${binary:Package}\\t${db:Status-Abbrev}\\t${Version}\\t${Installed-Size}\\n")
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		return nil, dbusutil.ToError(err)
	}
	err = cmd.Start()
	if err != nil {
		return nil, dbusutil.ToError(err)
	}
	defer func() {
		err = cmd.Wait()
		if err != nil {
			result = nil
			busErr = dbusutil.ToError(err)
		}
	}()

	apps, err := b.metadata.GetPackageApplicationCache()
	if nil != err {
		return nil, dbusutil.ToError(err)
	}

	scanner := bufio.NewScanner(stdout)
	for scanner.Scan() {
		parts := bytes.SplitN(scanner.Bytes(), []byte{'\t'}, 4)
		if len(parts) != 4 {
			continue
		}

		if bytes.HasPrefix(parts[1], []byte("ii")) {
			id := string(parts[0])
			fullPackageName := strings.Split(id, ":")
			app, ok := apps[fullPackageName[0]]
			if !ok {
				continue
			}
			fmt.Println(id)
			sizeStr := string(parts[3])
			size, _ := strconv.ParseInt(sizeStr, 10, 64)
			// unit of size is KiB, 1KiB = 1024Bytes

			t, _ := getInstallationTime(id)

			result = append(result, PackageInstalledInfo{
				ID:               string(parts[0]),
				Version:          string(parts[2]),
				InstalledSize:    size * 1024,
				LocaleName:       app.LocaleName,
				InstallationTime: t,
			})
		}
	}
	err = scanner.Err()
	if err != nil {
		return nil, dbusutil.ToError(err)
	}
	return result, nil
}

// PackageInstalledInfo store info of dpkg query
type PackageInstalledInfo struct {
	ID               string
	Version          string
	InstalledSize    int64 // unit byte
	InstallationTime int64
	LocaleName       map[string]string
}

// QueryVersion check package version info
func (b *Backend) QueryVersion(idList []string) (result []PackageVersionInfo,
	busErr *dbus.Error) {
	b.service.DelayAutoQuit()

	if 0 == len(idList) {
		return
	}

	args := append([]string{"policy", "--"}, idList...)
	cmd := exec.Command("/usr/bin/apt-cache", args...)
	cmd.Env = []string{"LC_ALL=C"}
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		return nil, dbusutil.ToError(err)
	}
	err = cmd.Start()
	if err != nil {
		return nil, dbusutil.ToError(err)
	}

	defer func() {
		err = cmd.Wait()
		if err != nil {
			result = nil
			busErr = dbusutil.ToError(err)
		}
	}()

	scanner := bufio.NewScanner(stdout)
	for scanner.Scan() {
		line := scanner.Bytes()
		if len(line) == 0 {
			continue
		}

		if line[0] != ' ' {
			// is package name line
			id := string(bytes.TrimRight(line, ":"))

			// get local version
			const installed = "Installed: "
			localVersion, err := scanVersion(scanner, installed)
			if err != nil {
				log.Println("can not find localVersion", string(line), idList)
				return nil, dbusutil.ToError(err)
			}

			// get remote version
			const candidate = "Candidate: "
			remoteVersion, err := scanVersion(scanner, candidate)
			if err != nil {
				log.Println("can not find remoteVersion", string(line))
				return nil, dbusutil.ToError(err)
			}

			result = append(result, PackageVersionInfo{
				ID:            id,
				LocalVersion:  localVersion,
				RemoteVersion: remoteVersion,
				Upgradable:    isUpgradable(localVersion, remoteVersion),
			})
		}
	}
	return result, nil
}

func scanVersion(scanner *bufio.Scanner, versionType string) (string, error) {
	ok := scanner.Scan()
	if !ok {
		return "", errors.New("scan failed")
	}
	line := scanner.Bytes()
	idx := bytes.Index(line, []byte(versionType))
	if idx == -1 {
		return "", errors.New("not found mark")
	}
	result := string(line[idx+len(versionType):])
	if result == "(none)" {
		result = ""
	}
	return result, nil
}

func isUpgradable(localVersion, remoteVersion string) bool {
	localVer, err := debVersion.NewVersion(localVersion)
	if err != nil {
		return false
	}

	remoteVer, err := debVersion.NewVersion(remoteVersion)
	if err != nil {
		return false
	}

	return remoteVer.GreaterThan(localVer)
}

// PackageVersionInfo store version info
type PackageVersionInfo struct {
	ID            string
	LocalVersion  string
	RemoteVersion string
	Upgradable    bool
}

// PackageInstallationTimeInfo store installed time info
type PackageInstallationTimeInfo struct {
	ID               string
	InstallationTime int64
}

// QueryInstallationTime store installed time of software
func (b *Backend) QueryInstallationTime(idList []string) (result []PackageInstallationTimeInfo,
	busErr *dbus.Error) {
	b.service.DelayAutoQuit()

	for _, id := range idList {
		t, err := getInstallationTime(id)
		if err == nil {
			result = append(result, PackageInstallationTimeInfo{
				ID:               id,
				InstallationTime: t,
			})
		} else {
			// log.Printf("warning: failed to get installation time of %q\n", id)
		}
	}
	return
}

func getInstallationTime(id string) (int64, error) {
	fileInfo, err := os.Stat("/var/lib/dpkg/info/" + id + ".md5sums")
	if err != nil {
		return 0, err
	}
	sysStat, ok := fileInfo.Sys().(*syscall.Stat_t)
	if !ok {
		return 0, errors.New("type assert failed")
	}
	t := time.Unix(int64(sysStat.Ctim.Sec), int64(sysStat.Ctim.Nsec))
	return t.Unix(), nil
}

// CleanArchives clean package cache
func (b *Backend) CleanArchives() *dbus.Error {
	b.service.DelayAutoQuit()

	_, err := b.lastore.CleanArchives(0)
	return dbusutil.ToError(err)
}
