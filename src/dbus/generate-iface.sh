#!/usr/bin/env bash

qdbusxml2cpp com.deepin.AppStore.xml \
  -a app_store_dbus_adapter \
  -c AppStoreDBusAdapter

qdbusxml2cpp com.deepin.AppStore.xml \
  -p app_store_dbus_interface \
  -c AppstoreDBusInterface