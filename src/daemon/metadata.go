package main

import (
	"encoding/json"
	"io/ioutil"
	"os"
	"sync"
	"time"
)

// Metadata store appstore server info
type Metadata struct {
	block      *blocklist
	debBackend *Backend
	settings   *Settings

	mutex sync.Mutex
	apps  map[string]*AppBody

	methods *struct {
		GetAppIcon         func() `in:"appName" out:"path"`
		GetAppMetadataList func() `in:"appNameList" out:"json"`
		OpenApp            func() `in:"appName"`
		OnMessage          func() `in:"playload"`
	}
}

// NewMetadata create new metadata with config
func NewMetadata() *Metadata {
	m := &Metadata{}
	m.apps = make(map[string]*AppBody)
	return m
}

func (m *Metadata) getAppIcon(appName string) string {
	iconFilepath := iconFolder + "/" + appName
	app, ok := m.apps[appName]
	if !ok {
		return ""
	}
	cacheFetch(m.settings.getMetadataServer()+"/"+app.Icon, iconFilepath, time.Hour*24*30)
	return iconFilepath
}

// func (m *Metadata) updateCache() {
// 	m.mutex.Lock()
// 	defer m.mutex.Unlock()

// 	if len(m.apps) > 0 {
// 		return
// 	}

// 	indexURL := m.settings.getOperationServer() + "/api/app"
// 	type indexResult struct {
// 		Apps []string `json:"apps"`
// 	}
// 	var index indexResult
// 	cacheFetchJSON(&index, indexURL, cacheFolder+"/index.json", time.Hour*24)

// 	putwayApps := make(map[string]int)
// 	for _, app := range index.Apps {
// 		putwayApps[app] = 1
// 	}

// 	indexURL = m.getMetadataServer() + "/api/app"
// 	result := AppResult{}
// 	cacheFetchJSON(&result, indexURL, cacheFolder+"/metadata.json", time.Hour*24)

// 	for _, app := range result.Apps {
// 		_, app.Putway = putwayApps[app.Name]
// 		m.apps[app.Name] = app
// 	}
// }

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
