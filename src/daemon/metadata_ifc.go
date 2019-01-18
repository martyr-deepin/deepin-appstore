package main

import (
	"encoding/json"
	"strings"

	dbus "pkg.deepin.io/lib/dbus1"
	"pkg.deepin.io/lib/utils"
)

const (
	dbusMetadataInterface = "com.deepin.AppStore.Metadata"
	dbusMetadataPath      = "/com/deepin/AppStore/Metadata"
)

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
	logger.Infof("receive message: %v", playload)
	action, ok := playload["action"]
	if !ok {
		logger.Errorf("unknown message %v", playload)
	}

	var err error
	switch action {
	case "install":
		err = m.handleInstall(playload)
	default:
		logger.Warning("unknown action %v", playload)
	}

	if nil != err {
		logger.Errorf("process message failed: %v", playload)
	}

	return nil
}

const (
	MetadataServer       = "MetadataServer"
	OperationServer      = "OperationServer"
	Region               = "Region"
	AutoInstall          = "AutoInstall"
	ThemeName            = "ThemeName"
	SupportSignIn        = "SupportSignIn"
	UpyunBannerVisible   = "UpyunBannerVisible"
	AllowSwitchRegion    = "AllowSwitchRegion"
	WindowState          = "WindowState"
	AllowShowPackageName = "AllowShowPackageName"
)

// SetSettings update dstore settings
func (m *Metadata) SetSettings(key string, value dbus.Variant) *dbus.Error {
	switch key {
	case AutoInstall:
		m.setUserSettings(groupGeneral, keyAutoInstall, value.Value())
	case Region:
		m.setUserSettings(groupGeneral, keyCurrentRegion, value.Value())
	case ThemeName:
		m.setUserSettings(groupGeneral, keyThemeName, value.Value())
	case WindowState:
		m.setUserSettings(groupWebWindow, keyWindowState, value.Value())
	}
	return nil
}

// GetSettings read setting of system and user
func (m *Metadata) GetSettings(key string) (dbus.Variant, *dbus.Error) {
	var ret dbus.Variant
	switch key {
	case MetadataServer:
		ret = dbus.MakeVariant(m.getMetadataServer())
	case OperationServer:
		ret = dbus.MakeVariant(m.getOperationServer())
	case AutoInstall:
		ret = dbus.MakeVariant(m.getAutoInstall())
	case Region:
		ret = dbus.MakeVariant(m.getRegion())
	case ThemeName:
		ret = dbus.MakeVariant(m.getThemeName())
	case SupportSignIn:
		ret = dbus.MakeVariant(m.getSupportSignIn())
	case UpyunBannerVisible:
		ret = dbus.MakeVariant(m.getUpyunBannerVisible())
	case AllowSwitchRegion:
		ret = dbus.MakeVariant(m.getAllowSwitchRegion())
	case WindowState:
		ret = dbus.MakeVariant(m.getWindowState())
	case AllowShowPackageName:
		ret = dbus.MakeVariant(m.getAllowShowPackageName())
	}
	// if ret.Value() == nil {
	// 	return dbus.Variant{}, dbus.NewError("GetSettings", []interface{}{"invalid key"})
	// }
	return ret, nil
}
