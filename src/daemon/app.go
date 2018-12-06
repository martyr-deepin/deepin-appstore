package main

import "time"

// form metadata server file model/app.go

// ImgType is int8
type ImgType int8

// AppBody parse app info
type AppBody struct {
	*AppInfo

	ID         uint      `json:"id,omitempty"`
	UserID     uint      `json:"userID,omitempty"`
	Name       string    `json:"name"`
	UpdateTime time.Time `json:"updateTime"`
	Putway     bool      `json:"putway"`

	LocaleDetail map[string]*AppLocale `json:"locale" binding:"required,checkDetail,dive"`

	SubmitterEmail string `json:"submitterEmail,omitempty"`
}

// AppLocale used by communication with web frontend.
type AppLocale struct {
	Description *AppDescription `json:"description" binding:"required,dive"`
	Version     []*AppVersion   `json:"versions" binding:"dive"`
	TagList     []string        `json:"tags" binding:"max=5"`
	Image       []*AppImage     `json:"images" binding:"required,dive"`
}

// AppInfo is ORM object
type AppInfo struct {
	ID         uint   `json:"-"`
	AppID      uint   `json:"-"`
	Author     string `json:"author"`
	Packager   string `json:"packager"`
	Category   string `json:"category" binding:"required" gorm:"not null"`
	HomePage   string `json:"homePage"`
	Icon       string `json:"icon" binding:"required" gorm:"not null"`
	PackageURI string `json:"packageURI" binding:"required,checkPackageUri" gorm:"not null"`
	Extra      string `json:"extra" gorm:"type:text"`
}

// AppDescription is ORM object
type AppDescription struct {
	ID          uint   `json:"-"`
	AppID       uint   `json:"-" gorm:"not null"`
	Locale      string `json:"-" gorm:"not null"`
	Name        string `json:"name" binding:"required,max=64" gorm:"not null"` // Localized application display name.
	Description string `json:"description" binding:"required" gorm:"not null;type:text"`
	Slogan      string `json:"slogan"`
}

// AppVersion is ORM object
type AppVersion struct {
	ID        uint   `json:"-"`
	AppID     uint   `json:"-" gorm:"not null;unique_index:version"`
	Locale    string `json:"-" gorm:"not null;unique_index:version"`
	Version   string `json:"version" binding:"required,max=64,checkVersion" gorm:"not null;unique_index:version"` // Human readable version name.
	Order     int    `json:"order" gorm:"not null"`
	ChangeLog string `json:"changeLog" binding:"required" gorm:"not null;type:text"`
}

// AppTag is ORM object
// TODO: Should not support json.
type AppTag struct {
	ID     uint
	AppID  uint   `gorm:"unique_index:tag"`
	Locale string `gorm:"unique_index:tag"`
	Tag    string `gorm:"unique_index:tag"`
}

// AppImage is ORM object
type AppImage struct {
	ID     uint    `json:"-"`
	AppID  uint    `json:"-"`
	Locale string  `json:"-"`
	Path   string  `json:"path" binding:"required" gorm:"not null"`
	Type   ImgType `json:"type" binding:"required" gorm:"not null"`
	Order  uint8   `json:"order"`
}

// AppResult is json data
type AppResult struct {
	Apps [](*AppBody) `json:"apps"`
}
