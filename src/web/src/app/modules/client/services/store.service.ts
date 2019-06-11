import { Injectable } from '@angular/core';
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

  InstalledPackages() {
    return this.execWithCallback<Package[]>('storeDaemon.installedPackages');
  }

  queryDownloadSize(param: QueryParam[]) {
    return this.execWithCallback<QueryResult>('storeDaemon.queryDownloadSize', param).pipe(
      map(result => {
        const arr = Object.values(result).filter(r => r && r.packages && r.packages.length > 0);
        return new Map(arr.map(pkg => [pkg.name, pkg.packages[0].downloadSize]));
      }),
    );
  }
  query(opts: QueryParam[]) {
    return this.execWithCallback<QueryResult>('storeDaemon.query', opts).pipe(
      map(results => {
        const arr = opts.map(opt => {
          const result = results[opt.name];
          if (!result) {
            return [opt.name, null] as [string, Package];
          }
          return [opt.name, result.packages.find(pkg => Boolean(pkg.appName))] as [string, Package];
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
    packages: Package[];
  };
}
export interface Package {
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
export interface QueryParam {
  name: string;
  localName: string;
  packages: { packageURI: string }[];
}
