package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
	"sync"
	"time"

	"github.com/go-ini/ini"
)

const (
	appstoreConfPath        = "/usr/share/deepin-appstore/settings.ini"
	appstoreConfPathDefault = "/usr/share/deepin-appstore/settings.ini.default"
)

// Metadata store appstore server info
type Metadata struct {
	sysCfg  *ini.File
	userCfg *ini.File

	block      *blocklist
	debBackend *Backend

	mutex sync.Mutex
	apps  map[string]*AppBody

	methods *struct {
		GetAppIcon         func() `in:"appName" out:"path"`
		GetAppMetadataList func() `in:"appNameList" out:"json"`
		OpenApp            func() `in:"appName"`
		OnMessage          func() `in:"playload"`
		GetSettings        func() `in:"key" out:"value"`
		SetSettings        func() `in:"key,value"`
	}
}

// NewMetadata create new metadata with config
func NewMetadata() *Metadata {
	var err error
	m := &Metadata{}

	m.apps = make(map[string]*AppBody)

	m.sysCfg, err = ini.Load(appstoreConfPath)
	if err != nil {
		logger.Info("fail to read file: %v", err)
		m.sysCfg, err = ini.Load(appstoreConfPathDefault)
		if err != nil {
			logger.Fatalf("fail to read file: %v", err)
		}
	}
	m.userCfg, err = ini.Load(configFolder + "/settings.ini")
	if err != nil {
		logger.Infof("fail to read user config file: %v", err)
		m.userCfg = ini.Empty()
	}
	return m
}

const (
	groupGeneral   = "General"
	groupWebWindow = "WebWindow"

	keyAutoInstall          = "autoInstall"
	keyCurrentRegion        = "currentRegion"
	keyThemeName            = "themeName"
	keyWindowState          = "windowState"
	keyAllowShowPackageName = "allowShowPackageName"

	metadataServer  = "metadataServer"
	operationServer = "operationServer"

	operationType            = "operationType"
	keySupportSigninName     = "supportSignIn"
	operationPrimaryServer   = "operationPrimary"
	operationSecondaryServer = "operationSecondary"
	keyOperationDefault      = "operationDefault"
)

const (
	regionChina         = 0
	regionInternational = 1
)

func (m *Metadata) getSystemSettings(key string) *ini.Key {
	return m.sysCfg.Section("General").Key(key)
}

func (m *Metadata) getMetadataServer() string {
	return m.sysCfg.Section("General").Key("metadataServer").String()
}

const (
	operationCommunity    = 0
	operationProfessional = 1
	operationLoongson     = 2
)

// Supported operation server types are:
//  * 0 - community
//  * 1 - professional
//  * 2 - loongson
func (m *Metadata) getOperationType() int {
	return m.sysCfg.Section(groupGeneral).Key("operationType").MustInt()
}

func (m *Metadata) getOperationServer() string {
	currentRegion := regionChina

	if m.userCfg != nil {
		currentRegion = m.userCfg.Section("General").Key("currentRegion").MustInt()
	}

	switch currentRegion {
	case regionChina:
		return m.sysCfg.Section("General").Key("operationPrimary").String()
	case regionInternational:
		return m.sysCfg.Section("General").Key("operationSecondary").String()
	}
	return ""
}

func (m *Metadata) getSupportSignIn() bool {
	return m.sysCfg.Section(groupGeneral).Key(keySupportSigninName).MustBool()
}

func (m *Metadata) getUpyunBannerVisible() bool {
	switch m.getOperationType() {
	case operationCommunity:
		return m.getRegion() == regionChina
	case operationLoongson:
		return false
	case operationProfessional:
		return false
	}
	return false
}

func (m *Metadata) getAllowSwitchRegion() bool {
	return m.getOperationType() == operationCommunity
}

func (m *Metadata) getWindowState() string {
	return m.userCfg.Section(groupWebWindow).Key(keyWindowState).MustString("")
}

func (m *Metadata) setUserSettings(group, key string, value interface{}) error {
	m.userCfg.Section(group).Key(key).SetValue(fmt.Sprint(value))
	m.userCfg.SaveTo(configFolder + "/settings.ini")
	return nil
}

func (m *Metadata) getUserSettings(group, key string) *ini.Key {
	return m.userCfg.Section(group).Key(key)
}

func (m *Metadata) getAutoInstall() bool {
	return m.getUserSettings(groupGeneral, keyAutoInstall).MustBool()
}

func (m *Metadata) getRegion() int {
	switch m.getOperationType() {
	case operationCommunity:
		defaultRegion := m.getSystemSettings(keyOperationDefault).MustInt()
		return m.getUserSettings(groupGeneral, keyCurrentRegion).MustInt(defaultRegion)
	}
	return regionChina
}

func (m *Metadata) getThemeName() string {
	return m.getUserSettings(groupGeneral, keyThemeName).MustString("light")
}

func (m *Metadata) getAllowShowPackageName() bool {
	return m.getUserSettings(groupGeneral, keyAllowShowPackageName).MustBool()
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
	m.mutex.Lock()
	defer m.mutex.Unlock()

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

type cacheAppInfo struct {
	Category    string            `json:"category"`
	PackageName string            `json:"package_name"`
	LocaleName  map[string]string `json:"locale_name"`
}

// 获取上架的apt缓存信息
func (m *Metadata) GetPackageApplicationCache() (apps map[string]*cacheAppInfo, err error) {
	apps = make(map[string]*cacheAppInfo)
	path := "/var/lib/lastore/applications.json"

	file, err := os.Open(path)
	if nil != err {
		return
	}
	data, err := ioutil.ReadAll(file)
	if nil != err {
		return
	}
	err = json.Unmarshal(data, &apps)

	return
}
