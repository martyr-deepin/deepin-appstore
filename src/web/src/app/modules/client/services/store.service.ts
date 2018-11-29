import { App } from 'app/services/app.service';
import { DownloadTotalService } from 'app/services/download-total.service';
import { Injectable } from '@angular/core';
import { Channel } from '../utils/channel';
import { Observable, from, of } from 'rxjs';
import { flatMap, map } from 'rxjs/operators';
import * as _ from 'lodash';

import { StoreJobInfo } from '../models/store-job-info';

@Injectable()
export class StoreService {
  constructor(private downloadTotalService: DownloadTotalService) {}

  isDBusConnected(): Observable<boolean> {
    return this.execWithCallback('storeDaemon.isDBusConnected');
  }

  getJobList(): Observable<string[]> {
    return this.execWithCallback('storeDaemon.jobList');
  }

  getJobInfo(jobPath: string): Observable<StoreJobInfo> {
    return this.execWithCallback('storeDaemon.getJobInfo', jobPath);
  }

  getJobsInfo(jobs: string[]): Observable<StoreJobInfo[]> {
    return this.execWithCallback('storeDaemon.getJobsInfo', jobs);
  }

  jobListChange(): Observable<string[]> {
    return Channel.connect('storeDaemon.jobListChanged');
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

  appInstallable(appName: string): Observable<boolean> {
    return this.execWithCallback('storeDaemon.packageInstallable', appName);
  }

  InstalledPackages(): Observable<string[]> {
    return this.execWithCallback<string[]>('storeDaemon.installedPackages');
  }

  fixError(errorType: string): Observable<string> {
    return this.execWithCallback('storeDaemon.fixError', errorType);
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
      packageSize: number;
      downloadSize: number;
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
