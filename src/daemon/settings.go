package main

import (
	"fmt"

	"github.com/go-ini/ini"
)

const (
	appstoreConfPath        = "/usr/share/deepin-appstore/settings.ini"
	appstoreConfPathDefault = "/usr/share/deepin-appstore/settings.ini.default"
)

const (
	gGeneral         = "General"
	gOperationServer = "OperationServer"
	gWebWindow       = "WebWindow"

	keyAutoInstall          = "autoInstall"
	keyCurrentRegion        = "currentRegion"
	keyThemeName            = "themeName"
	keyWindowState          = "windowState"
	keyAllowShowPackageName = "allowShowPackageName"

	keyMetadataServer     = "MetadataServer"
	keySupportSignIn      = "SupportSignIn"
	keyAllowSwitchRegion  = "AllowSwitchRegion"
	keyDefaultRegion      = "DefaultRegion"
	keyUpyunBannerVisible = "UpyunBannerVisible"
)

type Settings struct {
	sysCfg  *ini.File
	userCfg *ini.File

	methods *struct {
		GetSettings func() `in:"key" out:"value"`
		SetSettings func() `in:"key,value"`
	}
}

func NewSettings() *Settings {
	m := &Settings{}
	var err error
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

func (s *Settings) getSystemSettings(key string) *ini.Key {
	return s.sysCfg.Section(gGeneral).Key(key)
}

func (s *Settings) getMetadataServer() string {
	return s.sysCfg.Section(gGeneral).Key(keyMetadataServer).String()
}

func (s *Settings) getOperationServerMap() map[string]string {
	servers := make(map[string]string)
	keys := s.sysCfg.Section(gOperationServer).KeyStrings()
	for _, key := range keys {
		servers[key] = s.sysCfg.Section(gOperationServer).Key(key).String()
	}
	return servers
}

func (s *Settings) getSupportSignIn() bool {
	return s.sysCfg.Section(gGeneral).Key(keySupportSignIn).MustBool()
}

func (s *Settings) getUpyunBannerVisible() bool {
	return s.sysCfg.Section(gGeneral).Key(keyUpyunBannerVisible).MustBool()
}

func (s *Settings) getAllowSwitchRegion() bool {
	return s.sysCfg.Section(gGeneral).Key(keyAllowSwitchRegion).MustBool()
}

func (s *Settings) getWindowState() string {
	return s.userCfg.Section(gWebWindow).Key(keyWindowState).MustString("")
}

func (s *Settings) setUserSettings(group, key string, value interface{}) error {
	s.userCfg.Section(group).Key(key).SetValue(fmt.Sprint(value))
	s.userCfg.SaveTo(configFolder + "/settings.ini")
	return nil
}

func (s *Settings) getUserSettings(group, key string) *ini.Key {
	return s.userCfg.Section(group).Key(key)
}

func (s *Settings) getAutoInstall() bool {
	return s.getUserSettings(gGeneral, keyAutoInstall).MustBool()
}

func (s *Settings) getDefaultRegion() string {
	return s.sysCfg.Section(gGeneral).Key(keyDefaultRegion).MustString("Default")
}

func (s *Settings) getThemeName() string {
	return s.getUserSettings(gGeneral, keyThemeName).MustString("light")
}

func (s *Settings) getAllowShowPackageName() bool {
	return s.getUserSettings(gGeneral, keyAllowShowPackageName).MustBool()
}
