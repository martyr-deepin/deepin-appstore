package main

import (
	"encoding/json"
	"fmt"
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
	app, err := m.getAppMetadata(appName)
	if nil != err {
		return ""
	}
	fmt.Println(app.Icon)
	cacheFetch(m.settings.getMetadataServer()+"/"+app.Icon, iconFilepath, time.Hour*24*30)
	return iconFilepath
}

func (m *Metadata) getAppMetadata(appName string) (*AppBody, error) {
	type result struct {
		App AppBody `json:"app"`
	}
	ret := &result{}

	api := m.settings.getMetadataServer() + "/api/app/" + appName
	err := cacheFetchJSON(ret, api, cacheFolder+"/"+appName+".json", time.Hour*24)
	return &ret.App, err
}

type cacheAppInfo struct {
	Category    string            `json:"category"`
	PackageName string            `json:"package_name"`
	LocaleName  map[string]string `json:"locale_name"`
}

// GetPackageApplicationCache 获取上架的apt缓存信息
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
