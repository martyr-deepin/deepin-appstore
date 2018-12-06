package main

import (
	"encoding/json"
	"os"
	"strings"
	"time"

	"github.com/go-ini/ini"
	dbus "pkg.deepin.io/lib/dbus1"
	"pkg.deepin.io/lib/utils"
)

const (
	dbusMetadataInterface = "com.deepin.AppStore.Metadata"
	dbusMetadataPath      = "/com/deepin/AppStore/Metadata"
)

const (
	appstoreConfPath = "/usr/share/deepin-appstore/settings.ini"
)

// Metadata store appstore server info
type Metadata struct {
	cfg     *ini.File
	userCfg *ini.File

	apps map[string]*AppBody

	methods *struct {
		GetAppIcon         func() `in:"appName" out:"path"`
		GetAppMetadataList func() `in:"appNameList" out:"json"`
		OpenApp            func() `in:"appName"`
		OnMessage          func() `in:"playload"`
	}
}

// NewMetadata create new metadata with config
func NewMetadata() *Metadata {
	var err error
	m := &Metadata{}

	m.apps = make(map[string]*AppBody)

	m.cfg, err = ini.Load(appstoreConfPath)
	if err != nil {
		logger.Fatalf("Fail to read file: %v", err)
		os.Exit(1)
	}
	m.userCfg, err = ini.Load(configFolder + "/settings.ini")
	if err != nil {
		logger.Infof("Fail to read user config file: %v", err)
	}
	return m
}

func (m *Metadata) getMetadataServer() string {
	return m.cfg.Section("General").Key("metadataServer").String()
}

func (m *Metadata) getOperationServer() string {
	currentRegion := 0

	if m.userCfg != nil {
		currentRegion = m.userCfg.Section("General").Key("currentRegion").MustInt()
	}

	switch currentRegion {
	case 0:
		return m.cfg.Section("General").Key("operationPrimary").String()
	case 1:
		return m.cfg.Section("General").Key("operationSecondary").String()
	}
	return ""
}

func (m *Metadata) getAppIcon(appName string) string {
	iconFilepath := iconFolder + "/" + appName
	app, ok := m.apps[appName]
	if !ok {
		return ""
	}
	cacheFetch(m.getMetadataServer()+"/"+app.Icon, iconFilepath, time.Hour*24*30)
	return iconFilepath
}

func (m *Metadata) updateCache() {
	if len(m.apps) > 0 {
		return
	}

	indexURL := m.getOperationServer() + "/api/app"
	type indexResult struct {
		Apps []string `json:"apps"`
	}
	var index indexResult
	cacheFetchJSON(&index, indexURL, cacheFolder+"/index.json", time.Hour*24)

	putwayApps := make(map[string]int)
	for _, app := range index.Apps {
		putwayApps[app] = 1
	}

	indexURL = m.getMetadataServer() + "/api/app"
	result := AppResult{}
	cacheFetchJSON(&result, indexURL, cacheFolder+"/metadata.json", time.Hour*24)

	for _, app := range result.Apps {
		_, app.Putway = putwayApps[app.Name]
		m.apps[app.Name] = app
	}
}

// GetInterfaceName return dbus interface name
func (*Metadata) GetInterfaceName() string {
	return dbusMetadataInterface
}

// GetAppIcon return app local icon path
func (m *Metadata) GetAppIcon(appName string) (string, *dbus.Error) {
	m.updateCache()

	return m.getAppIcon(appName), nil
}

// GetAppMetadataList return app info with changelog
func (m *Metadata) GetAppMetadataList(appNameList []string) (string, *dbus.Error) {
	m.updateCache()

	appList := make([]*AppBody, 0)

	for _, name := range appNameList {
		appList = append(appList, m.apps[name])
	}

	data, _ := json.Marshal(appList)

	return string(data), nil
}

// OpenApp call lastore open app
func (m *Metadata) OpenApp(appName string) *dbus.Error {
	m.updateCache()

	output, _, err := utils.ExecAndWait(3600, "lastore-tools", "querydesktop", appName)
	if nil != err {
		logger.Errorf("call lastore-tools failed: %v", err)
		return dbus.NewError(err.Error(), nil)
	}
	output = strings.TrimSpace(output)

	if "" == output {
		logger.Infof("can not find desktop file")
		return dbus.NewError("no desktop file", nil)
	}

	sysBus, err := dbus.SessionBus()
	if nil != err {
		logger.Errorf("get dbus failed: %v", err)
		return dbus.NewError(err.Error(), nil)
	}
	startManager := sysBus.Object("com.deepin.SessionManager", "/com/deepin/StartManager")
	err = startManager.Call("com.deepin.StartManager.LaunchApp", 0, output, uint32(0), []string{}).Store()
	if nil != err {
		logger.Errorf("call dbus failed: %v", err)
		return dbus.NewError(err.Error(), nil)
	}
	return nil
}

// OnMessage handle push message
func (m *Metadata) OnMessage(playload map[string]interface{}) *dbus.Error {
	logger.Info(playload)
	return nil
}
