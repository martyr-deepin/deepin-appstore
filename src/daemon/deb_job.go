package main

import (
	"errors"
	"log"
	"sync"

	lastore "github.com/linuxdeepin/go-dbus-factory/com.deepin.lastore"
	dbus "pkg.deepin.io/lib/dbus1"
	"pkg.deepin.io/lib/dbusutil"
	"pkg.deepin.io/lib/dbusutil/proxy"
)

//go:generate dbusutil-gen -type Job -output job_dbusutil.go job.go

// Job for apt install task
type Job struct {
	service *dbusutil.Service
	core    *lastore.Job
	backend *Backend
	PropsMu sync.RWMutex

	Type         string
	Status       string
	Progress     float64
	Description  string
	Speed        int64
	Cancelable   bool
	DownloadSize int64

	// dbusutil-gen: ignore-below
	ID         string
	Name       string
	Packages   []string
	CreateTime int64
}

// GetInterfaceName return dbus interface name
func (*Job) GetInterfaceName() string {
	return dbusJobInterface
}

func (j *Job) getPath() dbus.ObjectPath {
	return dbus.ObjectPath(dbusJobPathPrefix + j.ID)
}

func newJob(backend *Backend, path dbus.ObjectPath) (*Job, error) {
	conn := backend.sysSigLoop.Conn()
	core, err := lastore.NewJob(conn, path)
	if err != nil {
		return nil, err
	}
	job := &Job{
		backend: backend,
		service: backend.service,
		core:    core,
	}

	job.ID, _ = core.Id().Get(0)
	if job.ID == "" {
		return nil, errors.New("job id empty")
	}
	job.Name, _ = core.Name().Get(0)
	job.Packages, _ = core.Packages().Get(0)
	log.Printf("newJob path %q, packages: %#v\n", path, job.Packages)
	job.CreateTime, _ = core.CreateTime().Get(0)
	job.Type, _ = core.Type().Get(0)
	job.Status, _ = core.Status().Get(0)
	job.Progress, _ = core.Progress().Get(0)
	job.Description, _ = core.Description().Get(0)
	job.Speed, _ = core.Speed().Get(0)
	job.DownloadSize, _ = core.DownloadSize().Get(0)
	job.Cancelable, _ = core.Cancelable().Get(0)

	core.InitSignalExt(backend.sysSigLoop, true)
	core.ConnectPropertiesChanged(func(interfaceName string,
		changedProperties map[string]dbus.Variant, invalidatedProperties []string) {

		job.PropsMu.Lock()
		defer job.PropsMu.Unlock()

		for propName, variant := range changedProperties {
			value := variant.Value()
			switch propName {
			case "Type":
				type0, ok := value.(string)
				if ok {
					job.setPropType(type0)
				}
			case "Status":
				status, ok := value.(string)
				if ok {
					job.setPropStatus(status)
				}
			case "Progress":
				progress, ok := value.(float64)
				if ok {
					job.setPropProgress(progress)
				}

			case "Description":
				desc, ok := value.(string)
				if ok {
					job.setPropDescription(desc)
				}

			case "Speed":
				speed, ok := value.(int64)
				if ok {
					job.setPropSpeed(speed)
				}

			case "Cancelable":
				cancelable, ok := value.(bool)
				if ok {
					job.setPropCancelable(cancelable)
				}

			case "DownloadSize":
				size, ok := value.(int64)
				if ok {
					job.setPropDownloadSize(size)
				}
			}
		}
	})

	return job, nil
}

// Start this job
func (j *Job) Start() *dbus.Error {
	err := j.backend.lastore.StartJob(dbus.FlagNoAutoStart, j.ID)
	return dbusutil.ToError(err)
}

// Pause this job
func (j *Job) Pause() *dbus.Error {
	err := j.backend.lastore.PauseJob(dbus.FlagNoAutoStart, j.ID)
	return dbusutil.ToError(err)
}

// Clean job cache
func (j *Job) Clean() *dbus.Error {
	err := j.backend.lastore.CleanJob(dbus.FlagNoAutoStart, j.ID)
	return dbusutil.ToError(err)
}

func (j *Job) destroy() {
	j.core.RemoveHandler(proxy.RemoveAllHandlers)
}
