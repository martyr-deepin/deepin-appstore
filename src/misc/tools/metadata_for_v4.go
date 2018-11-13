package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"time"
)

// form metadata server file model/app.go

type ImgType int8

type AppBody struct {
	Id         uint      `json:"id,omitempty"`
	UserId     uint      `json:"userID,omitempty"`
	Name       string    `json:"name"`
	UpdateTime time.Time `json:"updateTime"`

	*AppInfo
	LocaleDetail map[string]*AppLocale `json:"locale" binding:"required,checkDetail,dive"`

	SubmitterEmail string `json:"submitterEmail,omitempty"`
}

// Used by communication with web frontend.
type AppLocale struct {
	Description *AppDescription `json:"description" binding:"required,dive"`
	Version     []*AppVersion   `json:"versions" binding:"dive"`
	TagList     []string        `json:"tags" binding:"max=5"`
	Image       []*AppImage     `json:"images" binding:"required,dive"`
}

// ORM object
type AppInfo struct {
	Id         uint   `json:"-"`
	AppId      uint   `json:"-"`
	Author     string `json:"author"`
	Packager   string `json:"packager"`
	Category   string `json:"category" binding:"required" gorm:"not null"`
	HomePage   string `json:"homePage"`
	Icon       string `json:"icon" binding:"required" gorm:"not null"`
	PackageUri string `json:"packageURI" binding:"required,checkPackageUri" gorm:"not null"`
	Extra      string `json:"extra" gorm:"type:text"`
}

// ORM object
type AppDescription struct {
	Id          uint   `json:"-"`
	AppId       uint   `json:"-" gorm:"not null"`
	Locale      string `json:"-" gorm:"not null"`
	Name        string `json:"name" binding:"required,max=64" gorm:"not null"` // Localized application display name.
	Description string `json:"description" binding:"required" gorm:"not null;type:text"`
	Slogan      string `json:"slogan"`
}

// ORM object
type AppVersion struct {
	Id        uint   `json:"-"`
	AppId     uint   `json:"-" gorm:"not null;unique_index:version"`
	Locale    string `json:"-" gorm:"not null;unique_index:version"`
	Version   string `json:"version" binding:"required,max=64,checkVersion" gorm:"not null;unique_index:version"` // Human readable version name.
	Order     int    `json:"order" gorm:"not null"`
	ChangeLog string `json:"changeLog" binding:"required" gorm:"not null;type:text"`
}

// ORM object
// Should not support json.
type AppTag struct {
	Id     uint
	AppId  uint   `gorm:"unique_index:tag"`
	Locale string `gorm:"unique_index:tag"`
	Tag    string `gorm:"unique_index:tag"`
}

// ORM object
type AppImage struct {
	Id     uint    `json:"-"`
	AppId  uint    `json:"-"`
	Locale string  `json:"-"`
	Path   string  `json:"path" binding:"required" gorm:"not null"`
	Type   ImgType `json:"type" binding:"required" gorm:"not null"`
	Order  uint8   `json:"order"`
}

type AppResult struct {
	Apps [](*AppBody) `json:"apps"`
}

// manifest data format
type Manifest struct {
	ID             string                     `json:"id"` // deb package name
	Name           string                     `json:"name"`
	Description    string                     `json:"description"`
	ScreenShotList []*ScreenShot              `json:"screenshots"`
	IconList       []*Icon                    `json:"icons"`
	Locales        map[string]*ManifestLocale `json:"locales"`
	ChangeLog      map[string]string          `json:"changelog"`
}
type ManifestLocale struct {
	ID             string            `json:"id"` // deb package name
	Name           string            `json:"name"`
	Description    string            `json:"description"`
	ScreenShotList []*ScreenShot     `json:"screenshots"`
	ChangeLog      map[string]string `json:"changelog"`
}
type Icon struct {
	Size string `json:"size"`
	Src  string `json:"src"`
}
type ScreenShot struct {
	Size string `json:"size"`
	Src  string `json:"src"`
}

type DownloadInfo struct {
	AppName string `json:"appName"`
	Count   int    `json:"count"`
}

type RankResult struct {
	List [](*DownloadInfo) `json:"downloadCount"`
}

func rank(appMap map[string]*AppBody) {
	data, err := ioutil.ReadFile("appstat.json")
	if nil != err {
		log.Print("Red json failed", err)
		os.Exit(1)
	}

	rankResult := RankResult{}
	err = json.Unmarshal(data, &rankResult)
	if nil != err {
		log.Print("parse json failed", err)
		os.Exit(1)
	}
	downloadList := rankResult.List
	for idx, item := range downloadList {
		body, ok := appMap[item.AppName]
		if !ok {
			fmt.Printf("%v,%v,%v,%v,%v\n", idx, item.AppName, item.Count, "NULL", "NULL")
		} else {
			fmt.Printf("%v,%v,%v,%v,%v\n", idx, item.AppName, item.Count, body.Category, body.LocaleDetail["zh_CN"].Description.Name)
		}
	}
	return
}

func main() {
	data, err := ioutil.ReadFile("app.json")
	if nil != err {
		log.Print("Red json failed", err)
		os.Exit(1)
	}
	result := AppResult{}
	err = json.Unmarshal(data, &result)
	if nil != err {
		log.Print("Unmarshal json failed", err)
		os.Exit(1)
	}

	appMap := map[string]*AppBody{}
	for _, app := range result.Apps {
		appMap[app.Name] = app
	}

	for _, app := range result.Apps {
		locale := app.LocaleDetail
		if len(locale) <= 0 {
			continue
		}

		m := Manifest{
			Locales: make(map[string]*ManifestLocale, 0),
		}
		path := fmt.Sprintf("./metadata/%v/meta/manifest.json", app.Name)
		data, err := ioutil.ReadFile(path)
		if nil != err {
			log.Printf("read failed: %v %v", app.Name, err)
			os.MkdirAll(fmt.Sprintf("./metadata/%v/meta", app.Name), 0755)
		} else {
			err = json.Unmarshal(data, &m)
			if nil != err {
				log.Print(app.Name, err)
			}
		}

		needSync := false
		for localeStr, detail := range locale {
			manifestChangeLog := make(map[string]string, 0)
			manifestLocal := m.Locales[localeStr]
			if nil == manifestLocal {
				manifestLocal = &ManifestLocale{
					Name:        detail.Description.Name,
					Description: detail.Description.Description,
					ChangeLog:   manifestChangeLog,
				}
			}

			for _, changeLog := range detail.Version {
				needSync = true
				manifestLocal.ChangeLog[changeLog.Version] = changeLog.ChangeLog
			}

			m.Locales[localeStr] = manifestLocal
		}
		data, err = json.Marshal(m)
		if nil != err {
			log.Print(app.Name, err)
			continue
		}
		// if needSync {
		log.Printf("sync: %v", app.Name)
		ioutil.WriteFile(path, data, 0644)
		// }

	}
}
