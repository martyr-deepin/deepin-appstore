import { Injectable } from '@angular/core';
import { AppService } from './app.service';
import { Channel } from '../utils/channel';
import { Observable } from 'rxjs/Observable';
import { Observer } from 'rxjs/Observer';
import { StoreJobInfo } from './store-job-info';

@Injectable()
export class StoreService {

  constructor(private appService: AppService) {}

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
    return this.execWithCallback('dstoreDaemon.packageExists', appName);
  }

  /**
   * Check whether a package exists in local APT store and is able to be installed.
   * @param {string} appName
   * @returns {Observable<boolean>}
   */
  appInstallable(appName: string): Observable<boolean> {
    return this.execWithCallback('dstoreDaemon.packageInstallable', appName);
  }

  /**
   * Get size of package to be downloaded.
   * @param {string} appName
   * @returns {Observable<number>}
   */
  appDownloadSize(appName: string): Observable<number> {
    return this.execWithCallback('dstoreDaemon.packagesDownloadSize', appName);
  }

  /**
   * Get application list which are ready to update.
   * @returns {Observable<string[]>}
   */
  getUpgradableApps(): Observable<string[]> {
    return this.execWithCallback('dstoreDaemon.upgradableApps');
  }

  getJobInfo(jobPath: string): Observable<StoreJobInfo> {
    return this.execWithCallback('dstoreDaemon.getJobInfo', jobPath);
  }

  execWithCallback(method: string, ...args: any[]): Observable<any> {
    let response: any;
    const observable: Observable<any> = Observable.create((observer: Observer<any>) => {
      observer.next(response);
      observer.complete();
    });
    Channel.execWithCallback((dbusResponse: any) => response = dbusResponse,
      method, ...args);
    return observable;
  }
}
