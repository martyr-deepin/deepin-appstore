import { Injectable, NgZone, Version } from '@angular/core';
import { Channel } from '../utils/channel';
import { Observable, forkJoin, of } from 'rxjs';
import { flatMap, map, filter, take } from 'rxjs/operators';
import * as _ from 'lodash';

import { StoreJobInfo } from './store-job-info';
import { AppVersion } from './app-version';

interface SignalObject {
  connect: (any) => {};
  disconnect: () => {};
}

@Injectable()
export class StoreService {
  constructor(private zone: NgZone) {}
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
  installPackage(appName: string): Observable<string> {
    console.log('StoreService.installPackage()');
    return this.execWithCallback('storeDaemon.installPackage', appName);
  }

  /**
   * Update a spcific package. Call getUpgradableApps() first.
   * @param {string} appName
   * @returns {Observable<string>}
   */
  updatePackage(appName: string): Observable<string> {
    console.log('updatePackage: ', appName);
    return this.execWithCallback('storeDaemon.updatePackage', appName);
  }

  removePackage(appName: string): Observable<string> {
    console.log('removePackage: ', appName);
    return this.execWithCallback('storeDaemon.removePackage', appName);
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
    return this.execWithCallback('storeDaemon.queryVersions', appNameList.toString(), appNameList);
  }

  getJobInfo(jobPath: string): Observable<StoreJobInfo> {
    return this.execWithCallback('storeDaemon.getJobInfo', jobPath).pipe(
      map((jobInfo: StoreJobInfo) => {
        jobInfo.id = jobPath;
        return jobInfo;
      }),
    );
  }

  getJobByName(name: string): Observable<StoreJobInfo> {
    return this.getJobList().pipe(
      flatMap(jobs => (jobs.length > 0 ? forkJoin(jobs.map(job => this.getJobInfo(job))) : of([]))),
      map((jobInfoList: StoreJobInfo[]) => jobInfoList.find(info => info.name === name)),
    );
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
  getJobList(): Observable<string[]> {
    return this.execWithCallback('storeDaemon.jobList');
  }

  execWithCallback(method: string, ...args: any[]): Observable<any> {
    const obs$ = new Observable<StoreResponse>(obs => {
      return Channel.execWithCallback(
        (storeResp: StoreResponse) => {
          if (!storeResp.ok) {
            console.error('store error', storeResp);
          } else {
            console.warn('store resp', storeResp);
          }
          this.zone.run(() => obs.next(storeResp));
        },
        method,
        ...args,
      );
    });
    return obs$.pipe(
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
