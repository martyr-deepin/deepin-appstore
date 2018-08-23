import { Injectable, NgZone, Version } from '@angular/core';
import { Channel } from '../utils/channel';
import { Observable, forkJoin, of, BehaviorSubject } from 'rxjs';
import {
  flatMap,
  map,
  filter,
  take,
  switchMap,
  shareReplay,
  share,
  first,
  skip,
} from 'rxjs/operators';
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
  private jobList$ = new BehaviorSubject<string[]>([]);

  constructor(private zone: NgZone, private http: HttpClient) {
    this.initJobListSub();
  }

  private initJobListSub() {
    const callback = (list: string[]) => this.jobList$.next(list);
    this.execWithCallback('storeDaemon.jobList').subscribe(callback);
    Channel.registerCallback('storeDaemon.jobListChanged', callback);
  }

  getJobList(): Observable<string[]> {
    return this.jobList$.asObservable().pipe(first());
  }

  jobListChange(): Observable<string[]> {
    return this.jobList$.asObservable().pipe(skip(1));
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
    return this.execWithCallback<AppVersion[]>(
      'storeDaemon.queryVersions',
      appNameList.toString(),
      appNameList,
    ).pipe(map(versionList => versionList.filter(v => v.remoteVersion)));
  }

  getVersionMap(appNameList: string[]): Observable<Map<string, AppVersion>> {
    return this.execWithCallback(
      'storeDaemon.queryVersions',
      appNameList.toString(),
      appNameList,
    ).pipe(map((vs: AppVersion[]) => new Map(_.toPairs(_.keyBy(vs, 'name')))));
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
    return this.execWithCallback(
      'storeDaemon.queryInstalledTime',
      appNameList.toString(),
      appNameList,
    );
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
    return this.execWithCallback('storeDaemon.getJobsInfo', jobs.join(','), jobs);
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
    const obs$ = new Observable<StoreResponse>(obs => {
      return Channel.execWithCallback(
        (storeResp: StoreResponse) => {
          if (!storeResp.ok) {
            console.error(method, 'store error', storeResp);
          } else {
            console.warn(method, 'store resp', storeResp);
          }
          this.zone.run(() => obs.next(storeResp));
        },
        method,
        ...args,
      );
    });
    return obs$.pipe(
      filter(resp => resp.errorName === ''),
      filter(resp => args.length === 0 || args[0] === resp.result.name),
      map(resp => resp.result.value),
      take(1),
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
  result: { name: string; value: any };
}
