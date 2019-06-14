package main

import (
	dbus "pkg.deepin.io/lib/dbus1"
)

const (
	dbusSettingsInterface = "com.deepin.AppStore.Settings"
	dbusSettingsPath      = "/com/deepin/AppStore/Settings"
)

// GetInterfaceName return dbus interface name
func (*Settings) GetInterfaceName() string {
	return dbusSettingsInterface
}

const (
	AutoInstall          = "AutoInstall"
	ThemeName            = "ThemeName"
	WindowState          = "WindowState"
	AllowShowPackageName = "AllowShowPackageName"

	MetadataServer     = "MetadataServer"
	OperationServerMap = "OperationServerMap"
	DefaultRegion      = "DefaultRegion"
	AllowSwitchRegion  = "AllowSwitchRegion"
	SupportSignIn      = "SupportSignIn"
	SupportAot         = "SupportAot"
	UpyunBannerVisible = "UpyunBannerVisible"
)

// SetSettings update dstore settings
func (s *Settings) SetSettings(key string, value dbus.Variant) *dbus.Error {
	switch key {
	case AutoInstall:
		s.setUserSettings(gGeneral, keyAutoInstall, value.Value())
	case ThemeName:
		s.setUserSettings(gGeneral, keyThemeName, value.Value())
	case WindowState:
		s.setUserSettings(gWebWindow, keyWindowState, value.Value())
	}
	return nil
}

// GetSettings read setting of system and user
func (s *Settings) GetSettings(key string) (dbus.Variant, *dbus.Error) {
	var ret dbus.Variant
	switch key {
	case MetadataServer:
		ret = dbus.MakeVariant(s.getMetadataServer())
	case OperationServerMap:
		ret = dbus.MakeVariant(s.getOperationServerMap())
	case AutoInstall:
		ret = dbus.MakeVariant(s.getAutoInstall())
	case DefaultRegion:
		ret = dbus.MakeVariant(s.getDefaultRegion())
	case ThemeName:
		ret = dbus.MakeVariant(s.getThemeName())
	case SupportSignIn:
		ret = dbus.MakeVariant(s.getSupportSignIn())
	case UpyunBannerVisible:
		ret = dbus.MakeVariant(s.getUpyunBannerVisible())
	case AllowSwitchRegion:
		ret = dbus.MakeVariant(s.getAllowSwitchRegion())
	case WindowState:
		ret = dbus.MakeVariant(s.getWindowState())
	case AllowShowPackageName:
		ret = dbus.MakeVariant(s.getAllowShowPackageName())
	case SupportAot:
		ret = dbus.MakeVariant(s.getSupportAot())
	}
	// if ret.Value() == nil {
	// 	return dbus.Variant{}, dbus.NewError("GetSettings", []interface{}{"invalid key"})
	// }
	return ret, nil
}
