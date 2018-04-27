import { Injectable, NgZone } from '@angular/core';
import { AppService } from './app.service';
import { Channel } from '../utils/channel';
import { Observable } from 'rxjs/Observable';
import { Observer } from 'rxjs/Observer';
import 'rxjs/add/observable/fromPromise';
import 'rxjs/add/observable/empty';
import 'rxjs/add/operator/filter';
import * as _ from 'lodash';

import { StoreJobInfo } from './store-job-info';

interface SignalObject {
  connect: (any) => {};
  disconnect: () => {};
}

@Injectable()
export class StoreService {
  constructor(private appService: AppService, private zone: NgZone) {}
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

  getJobInfo(jobPath: string): Observable<StoreJobInfo> {
    return this.execWithCallback('storeDaemon.getJobInfo', jobPath);
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
    const obs$ = Observable.create(obs => {
      return Channel.execWithCallback(
        (storeResp: StoreResponse) => {
          if (!storeResp.ok) {
            throw storeResp;
          }
          this.zone.run(() => obs.next(storeResp));
        },
        method,
        ...args,
      );
    }) as Observable<StoreResponse>;
    return obs$
      .filter(resp => args.length === 0 || args[0] === resp.result.name)
      .map(resp => resp.result.value);
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
