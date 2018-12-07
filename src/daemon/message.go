package main

import (
	"strings"

	"github.com/mitchellh/mapstructure"
)

/*
{
    "type": "store",
    "playload": {
        "action": "install",
        "apps": [{
            "name": "gedit",
            "packageURI": "dpk://deb/gedit"
        }, {
            "name": "deepin-music",
            "packageURI": "dpk://deb/deepin-music"
        }]
    }
}
*/

type msgInstall struct {
	Action string `json:"action"`
	Apps   []struct {
		Name       string `json:"name"`
		PackageURI string `json:"packageURI"`
	} `json:"apps"`
}

func (m *Metadata) handleInstall(playload map[string]interface{}) error {
	var msg msgInstall
	err := mapstructure.Decode(playload, &msg)
	if nil != err {
		logger.Errorf("decode message %v failed: %v", playload, err)
		return err
	}

	for _, app := range msg.Apps {
		packageName := strings.Replace(app.PackageURI, "dpk://deb/", "", -1)
		logger.Infof("install %v %v", app.Name, packageName)
		m.debBackend.Install(app.Name, packageName)
	}

	return nil
}
