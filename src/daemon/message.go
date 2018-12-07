package main

import (
	"encoding/json"
	"io/ioutil"
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
		Name           string   `json:"name"`
		PackageURIList []string `json:"package_uri_list"`
	} `json:"apps"`
}

func (m *Metadata) handleInstall(playload map[string]interface{}) error {
	var msg msgInstall
	err := mapstructure.Decode(playload, &msg)
	if nil != err {
		logger.Errorf("decode message %v failed: %v", playload, err)
		return err
	}

	// TODO: support flatpak or multi format
	for _, app := range msg.Apps {
		for _, dpk := range app.PackageURIList {
			packageName := strings.Replace(dpk, "dpk://deb/", "", -1)
			if _, ok := m.block.list[packageName]; ok {
				logger.Infof("skip user remove package")
				continue
			}
			logger.Infof("install %v %v", app.Name, packageName)
			m.debBackend.Install(app.Name, packageName)
		}
	}

	return nil
}

type blocklist struct {
	blockFilepath string
	list          map[string]string
}

func newBlocklist() *blocklist {
	b := &blocklist{
		blockFilepath: configFolder + "/block.list",
		list:          make(map[string]string),
	}
	b.loadBlocklist()
	return b
}

func (b *blocklist) add(id string) {
	b.list[id] = ""
	b.saveBlocklist()
}

func (b *blocklist) remove(id string) {
	delete(b.list, id)
	b.saveBlocklist()
}

// if user remove and package, save it to block list
func (b *blocklist) loadBlocklist() {
	data, _ := ioutil.ReadFile(b.blockFilepath)
	json.Unmarshal(data, &b.list)
	return
}

func (b *blocklist) saveBlocklist() {
	data, _ := json.Marshal(b.list)
	ioutil.WriteFile(b.blockFilepath, data, 0644)
}
