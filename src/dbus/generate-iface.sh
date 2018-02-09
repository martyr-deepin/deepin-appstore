#!/usr/bin/env bash

qdbusxml2cpp com.deepin.AppStore.xml \
  -a app_store_dbus_adapter \
  -c AppStoreDBusAdapter

qdbusxml2cpp com.deepin.AppStore.xml \
  -p app_store_dbus_interface \
  -c AppStoreDBusInterface

qdbusxml2cpp com.deepin.lastore.manager.xml \
  -p lastore_manager_interface \
  -i dbus/dbusvariant/app_update_info.h \
  -c LastoreManagerInterface

qdbusxml2cpp com.deepin.lastore.updater.xml \
  -p lastore_updater_interface \
  -i dbus/dbusvariant/app_update_info.h \
  -c LastoreUpdaterInterface