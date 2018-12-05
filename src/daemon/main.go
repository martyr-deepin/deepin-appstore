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

	b, err := newBackend(service)
	if err != nil {
		logger.Fatal(err)
	}
	b.init()

	err = service.Export(dbusBackendPath, b)
	if err != nil {
		logger.Fatal(err)
	}

	m := NewMetadata()
	err = service.Export(dbusMetadataPath, m)
	if err != nil {
		logger.Fatal(err)
	}
	m.updateCache()

	err = service.RequestName(dbusServiceName)
	if err != nil {
		logger.Fatal(err)
	}

	service.SetAutoQuitHandler(3*time.Minute, func() bool {
		// b.PropsMu.Lock()
		// jobCount := len(b.jobs)
		// b.PropsMu.Unlock()
		// DO NOT auto quit
		return false
		// return jobCount == 0
	})

	service.Wait()
}
