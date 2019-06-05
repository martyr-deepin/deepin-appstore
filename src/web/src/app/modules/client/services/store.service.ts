import { Injectable } from '@angular/core';
import { App } from 'app/services/app.service';
import { DownloadTotalService } from 'app/services/download-total.service';
import { Channel } from '../utils/channel';
import { Observable, from, of } from 'rxjs';
import { flatMap, map } from 'rxjs/operators';
import * as _ from 'lodash';

import { StoreJobInfo } from '../models/store-job-info';

@Injectable({
  providedIn: 'root',
})
export class StoreService {
  constructor(private downloadTotalService: DownloadTotalService) {}

  isDBusConnected(): Observable<boolean> {
    return this.execWithCallback('storeDaemon.isDBusConnected');
  }

  fixError(errorType: string): Observable<string> {
    return this.execWithCallback('storeDaemon.fixError', errorType);
  }

  getJobList(): Observable<string[]> {
    return this.execWithCallback('storeDaemon.jobList');
  }

  getJobInfo(jobPath: string): Observable<StoreJobInfo> {
    return this.execWithCallback('storeDaemon.getJobInfo', jobPath);
  }

  getJobsInfo(jobPaths: string[]): Observable<StoreJobInfo[]> {
    return this.execWithCallback('storeDaemon.getJobsInfo', jobPaths);
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

  getAllowShowPackageName(): Promise<boolean> {
    return Channel.exec('settings.allowShowPackageName');
  }

  openApp(app: App): void {
    Channel.exec('storeDaemon.openApp', this.toQuery(app));
  }
  installPackages(apps: App[]): Observable<string> {
    this.downloadTotalService.installed(apps);
    return this.execWithCallback(
      'storeDaemon.installPackages',
      apps.map(this.toQuery),
    );
  }
  updatePackages(apps: App[]): Observable<string> {
    this.downloadTotalService.installed(apps);
    return this.execWithCallback(
      'storeDaemon.updatePackages',
      apps.map(this.toQuery),
    );
  }
  removePackages(apps: App[]): Observable<string> {
    return this.execWithCallback(
      'storeDaemon.removePackages',
      apps.map(this.toQuery),
    );
  }
  InstalledPackages() {
    interface InstalledPackage {
      packageURI: string;
      size: number;
    }
    return this.execWithCallback<InstalledPackage[]>(
      'storeDaemon.installedPackages',
    ).pipe(
      map(result =>
        result.reduce(
          (m, pkg) => m.set(pkg.packageURI, pkg),
          new Map<string, InstalledPackage>(),
        ),
      ),
    );
  }

  toQuery(app: App) {
    return {
      name: app.name,
      localName: app.localInfo.description.name,
      packages: app.packageURI.map(packageURI => ({ packageURI })),
    };
  }

  queryDownloadSize(apps: App[]) {
    return this.execWithCallback<QueryResult>(
      'storeDaemon.queryDownloadSize',
      apps.map(this.toQuery),
    ).pipe(
      map(result => {
        const arr = Object.values(result)
          .filter(Boolean)
          .filter(r => r.packages && r.packages.length > 0)
          .map(r => [r.name, r.packages[0].downloadSize] as [string, number]);
        return new Map(arr);
      }),
    );
  }
  queryPackage(apps: App[]) {
    return this.execWithCallback<QueryResult>(
      'storeDaemon.query',
      apps.map(this.toQuery),
    ).pipe(
      map(result => {
        const arr = Object.values(result)
          .filter(Boolean)
          .map(r => r.packages.find(pkg => Boolean(pkg.appName)))
          .filter(Boolean)
          .map(pkg => [pkg.appName, pkg] as [string, AppPackage]);
        return new Map(arr);
      }),
    );
  }

  query(
    opts: {
      name: string;
      localName: string;
      packages: { packageURI: string }[];
    }[],
  ) {
    return this.execWithCallback<QueryResult>('storeDaemon.query', opts).pipe(
      map(results => {
        const arr = opts.map(opt => {
          const result = results[opt.name];
          if (!result) {
            return [opt.name, null] as [string, AppPackage];
          }
          return [
            opt.name,
            result.packages.find(pkg => Boolean(pkg.appName)),
          ] as [string, AppPackage];
        });
        return new Map(arr);
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

interface QueryResult {
  [key: string]: {
    name: string;
    packages: AppPackage[];
  };
}
interface AppPackage {
  appName: string;
  packageName: string;
  packageURI: string;
  localVersion: string;
  remoteVersion: string;
  upgradable: boolean;
  installedTime: number;
  downloadSize: number;
  packageSize: number;
}
