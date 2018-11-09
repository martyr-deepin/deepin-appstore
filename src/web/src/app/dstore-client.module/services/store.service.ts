import { Injectable, NgZone, Version } from '@angular/core';
import { Channel } from '../utils/channel';
import { Observable, from, of, BehaviorSubject } from 'rxjs';
import { flatMap, map, first, skip } from 'rxjs/operators';
import * as _ from 'lodash';

import { StoreJobInfo } from '../models/store-job-info';
import { AppVersion } from '../models/app-version';
import { InstalledApp } from '../models/installed';
import { HttpClient } from '@angular/common/http';
import { BaseService } from '../../dstore/services/base.service';

interface SignalObject {
  connect: (any) => {};
  disconnect: () => {};
}

@Injectable()
export class StoreService {
  private server = BaseService.serverHosts.operationServer;

  constructor(private zone: NgZone, private http: HttpClient) {}

  getJobList(): Observable<string[]> {
    return this.execWithCallback('storeDaemon.jobList');
  }

  jobListChange(): Observable<string[]> {
    return Channel.connect('storeDaemon.jobListChanged');
  }

  private downloadRecord(appName: string) {
    this.http.post<void>(`${this.server}/api/downloading/app/${appName}`, null).subscribe();
  }
  /**
   * Check connectivity to backend lastore daemon.
   * @returns {Observable<boolean>} If returns false, all methods in this class will not work.
   */
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

  /**
   * Install a specific package.
   * @param {string} appName
   * @returns {Observable<string>} path to job
   */
  installPackage(appName: string, localName: string): Observable<string> {
    this.downloadRecord(appName);
    return this.execWithCallback('storeDaemon.installPackage', appName, localName);
  }

  /**
   * Update a spcific package. Call getUpgradableApps() first.
   * @param {string} appName
   * @returns {Observable<string>}
   */
  updatePackage(appName: string, localName: string): Observable<string> {
    this.downloadRecord(appName);
    return this.execWithCallback('storeDaemon.updatePackage', appName, localName);
  }

  removePackage(appName: string, localName: string): Observable<string> {
    return this.execWithCallback('storeDaemon.removePackage', appName, localName);
  }

  /**
   * Check whether application installed or not.
   * @param {string} appName
   * @returns {Observable<boolean>}
   */
  appInstalled(appName: string): Observable<boolean> {
    return this.execWithCallback('storeDaemon.packageExists', appName);
  }

  /**
   * Check whether a package exists in local APT store and is able to be installed.
   * @param {string} appName
   * @returns {Observable<boolean>}
   */
  appInstallable(appName: string): Observable<boolean> {
    return this.execWithCallback('storeDaemon.packageInstallable', appName);
  }

  /**
   * Get size of package to be downloaded.
   * @param {string} appName
   * @returns {Observable<number>}
   */
  appDownloadSize(appName: string): Observable<number> {
    return this.execWithCallback('storeDaemon.packageDownloadSize', appName);
  }

  /**
   * Get application list which are ready to update.
   * @returns {Observable<string[]>}
   */
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

  /**
   * Request to run application in background.
   * @param {string} appName
   */
  openApp(appName: string): void {
    Channel.exec('storeDaemon.openApp', appName);
  }

  /**
   * Get all of jobs in backend.
   * @returns {Observable<string[]>}
   */

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
