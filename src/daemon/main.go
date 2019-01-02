package main

import (
	"time"

	"pkg.deepin.io/lib/dbusutil"
	"pkg.deepin.io/lib/log"
)

var logger = log.NewLogger("deepin-appstore-daemon")

const (
	dbusServiceName = "com.deepin.AppStore.Daemon"
	dbusBackendPath = "/com/deepin/AppStore/Backend"
)

func main() {
	service, err := dbusutil.NewSessionService()
	if err != nil {
		logger.Fatal(err)
	}

	block := newBlocklist()

	b, err := newBackend(service)
	if err != nil {
		logger.Fatal(err)
	}
	b.init()
	b.block = block

	err = service.Export(dbusBackendPath, b)
	if err != nil {
		logger.Fatal(err)
	}

	m := NewMetadata()
	m.debBackend = b
	m.block = block

	err = service.Export(dbusMetadataPath, m)
	if err != nil {
		logger.Fatal(err)
	}
	go m.updateCache()

	// caller := service.Conn().Object("com.deepin.pusher", "/com/deepin/pusher")
	// err = caller.Call("com.deepin.pusher.Register", 0, "store", dbusServiceName, dbusMetadataPath, dbusMetadataInterface).Store()
	// if nil != err {
	// 	logger.Errorf("register message handler failed: %v", err)
	// }

	err = service.RequestName(dbusServiceName)
	if err != nil {
		logger.Fatal(err)
	}

	logger.Infof("start deepin-appstore-daemon")
	service.SetAutoQuitHandler(3*time.Minute, func() bool {
		b.PropsMu.Lock()
		jobCount := len(b.jobs)
		b.PropsMu.Unlock()
		return jobCount == 0
	})

	service.Wait()
}
