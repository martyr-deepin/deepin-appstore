import { App } from 'app/services/app.service';
import { DownloadTotalService } from 'app/services/download-total.service';
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

  getUpgradableApps(): Observable<string[]> {
    return this.execWithCallback('storeDaemon.upgradableApps');
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

  queryPackage(apps: App[]) {
    interface AppPackage {
      appName: string;
      packageName: string;
      packageURI: string;
      localVersion: string;
      remoteVersion: string;
      upgradable: boolean;
      installedTime: number;
      size: number;
    }
    interface Result {
      [key: string]: {
        name: string;
        packages: AppPackage[];
      };
    }
    const query = apps.map(app => {
      return {
        name: app.name,
        packages: app.packageURI.map(url => {
          return { packageURI: url };
        }),
      };
    });
    return this.execWithCallback<Result>('storeDaemon.query', query).pipe(
      map(result => {
        const pkg = new Map<string, AppPackage>();
        for (const r of Object.values(result)) {
          if (r.packages.length > 0) {
            pkg.set(r.name, r.packages[0]);
          }
        }
        return pkg;
      }),
    );
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
