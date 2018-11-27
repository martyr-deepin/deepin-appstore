import { DownloadTotalService } from './../../../services/download-total.service';
import { Injectable } from '@angular/core';
import { Channel } from '../utils/channel';
import { Observable, from, of } from 'rxjs';
import { flatMap, map } from 'rxjs/operators';
import * as _ from 'lodash';

import { StoreJobInfo } from '../models/store-job-info';
import { AppVersion } from '../models/app-version';
import { InstalledApp } from '../models/installed';

@Injectable()
export class StoreService {
  constructor(private downloadTotalService: DownloadTotalService) {}

  getJobList(): Observable<string[]> {
    return this.execWithCallback('storeDaemon.jobList');
  }

  jobListChange(): Observable<string[]> {
    return Channel.connect('storeDaemon.jobListChanged');
  }

  isDBusConnected(): Observable<boolean> {
    return this.execWithCallback('storeDaemon.isDBusConnected');
  }

  clearJob(job: string): void {
    Channel.exec('storeDaemon.cleanJob', job);
  }

  pauseJob(job: string): void {
    Channel.exec('storeDaemon.pauseJob', job);
  }

  resumeJob(job: string): void {
    Channel.exec('storeDaemon.startJob', job);
  }

  installPackage(appName: string, localName: string): Observable<string> {
    this.downloadTotalService.installed(appName);
    return this.execWithCallback('storeDaemon.installPackage', appName, localName);
  }

  updatePackage(appName: string, localName: string): Observable<string> {
    this.downloadTotalService.installed(appName);
    return this.execWithCallback('storeDaemon.updatePackage', appName, localName);
  }

  removePackage(appName: string, localName: string): Observable<string> {
    return this.execWithCallback('storeDaemon.removePackage', appName, localName);
  }

  appInstalled(appName: string): Observable<boolean> {
    return this.execWithCallback('storeDaemon.packageExists', appName);
  }

  appInstallable(appName: string): Observable<boolean> {
    return this.execWithCallback('storeDaemon.packageInstallable', appName);
  }

  appDownloadSize(appName: string): Observable<number> {
    return this.execWithCallback('storeDaemon.packageDownloadSize', appName);
  }

  getUpgradableApps(): Observable<string[]> {
    return this.execWithCallback('storeDaemon.upgradableApps');
  }

  getVersion(appNameList: string[]): Observable<AppVersion[]> {
    if (appNameList.length === 0) {
      return of([]);
    }
    return this.execWithCallback<AppVersion[]>('storeDaemon.queryVersions', appNameList).pipe(
      map(versionList => versionList.filter(v => v.remoteVersion)),
    );
  }

  getVersionMap(appNameList: string[]): Observable<Map<string, AppVersion>> {
    return this.getVersion(appNameList).pipe(
      map((vs: AppVersion[]) => {
        return new Map(vs.map(v => [v.name, v] as [string, AppVersion]));
      }),
    );
  }

  getInstalledApps(): Observable<InstalledApp[]> {
    return this.execWithCallback<InstalledApp[]>('storeDaemon.installedPackages').pipe(
      flatMap(
        apps => this.getInstalledTimes(apps.map(app => app.name)),
        (apps, times) => {
          apps.forEach(
            app =>
              (app.time = _.chain(times)
                .find({ app: app.name })
                .get('time')
                .value()),
          );
          return apps;
        },
      ),
    );
  }

  getInstalledTimes(appNameList: string[]): Observable<{ app: string; time: number }[]> {
    return this.execWithCallback('storeDaemon.queryInstalledTime', appNameList);
  }

  getInstalledTimeMap(appNameList: string[]): Observable<Map<string, number>> {
    return this.getInstalledTimes(appNameList).pipe(
      map(
        installedTime =>
          new Map(
            _.chain(installedTime)
              .keyBy('app')
              .mapValues('time')
              .entries()
              .value(),
          ),
      ),
    );
  }

  fixError(errorType: string): Observable<string> {
    return this.execWithCallback('storeDaemon.fixError', errorType);
  }

  getJobInfo(jobPath: string): Observable<StoreJobInfo> {
    return this.execWithCallback('storeDaemon.getJobInfo', jobPath);
  }

  getJobsInfo(jobs: string[]): Observable<StoreJobInfo[]> {
    return this.execWithCallback('storeDaemon.getJobsInfo', jobs);
  }

  openApp(appName: string): void {
    Channel.exec('storeDaemon.openApp', appName);
  }

  execWithCallback<T>(method: string, ...args: any[]): Observable<T> {
    return from(
      Channel.exec<StoreResponse>(method, ...args).then(resp => {
        return resp.ok ? resp.result : Promise.reject(resp);
      }),
    );
  }
}

/**
 * Returned value from store daemon proxy dbus interface.
 */
class StoreResponse {
  ok: boolean;
  errorName: string;
  errorMsg: string;
  result: any;
}
