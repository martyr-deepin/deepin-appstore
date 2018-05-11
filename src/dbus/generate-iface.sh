#!/usr/bin/env bash

qdbusxml2cpp com.deepin.AppStore.xml \
  -a app_store_dbus_adapter \
  -c AppStoreDBusAdapter

qdbusxml2cpp com.deepin.AppStore.xml \
  -p app_store_dbus_interface \
  -c AppStoreDBusInterface

qdbusxml2cpp com.deepin.AppStore.Metadata.xml \
  -a app_store_metadata_dbus_adapter \
  -i dbus/dbus_variant/app_metadata.h \
  -c AppStoreMetadataDBusAdapter

qdbusxml2cpp com.deepin.AppStore.Backend.Deb.xml \
  -p lastore_deb_interface \
  -i dbus/dbus_variant/app_version.h \
  -i dbus/dbus_variant/installed_app_info.h \
  -c LastoreDebInterface

qdbusxml2cpp com.deepin.AppStore.Backend.Job.xml \
  -p lastore_job_interface \
  -c LastoreJobInterface