import { Component, OnInit } from '@angular/core';
import { Observable, of, forkJoin, timer, iif, merge } from 'rxjs';
import {
  flatMap,
  defaultIfEmpty,
  map,
  tap,
  publishReplay,
  refCount,
  switchMap,
  distinctUntilChanged,
} from 'rxjs/operators';

import { memoize, throttle, sortBy } from 'lodash';

import { App } from '../../../dstore/services/app';
import { AppService } from '../../../services/app.service';
import { BaseService } from '../../../dstore/services/base.service';
import { StoreService } from '../../../dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
} from '../../../dstore-client.module/models/store-job-info';

@Component({
  selector: 'app-download',
  templateUrl: './download.component.html',
  styleUrls: ['./download.component.scss'],
})
export class DownloadComponent implements OnInit {
  metadataServer = BaseService.serverHosts.metadataServer;
  constructor(private appService: AppService, private storeService: StoreService) {}

  StoreJobType = StoreJobType;
  StoreJobStatus = StoreJobStatus;

  // 下载任务控制
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  cancel = this.storeService.clearJob;

  jobs$: Observable<JonInfoRx[]>;
  ngOnInit() {
    this.jobs$ = merge(this.storeService.getJobList(), this.storeService.jobListChange()).pipe(
      map(jobs => jobs.filter(job => job.includes('install'))),
      map(jobs => {
        return jobs.map(job => {
          const job$ = timer(0, 1000).pipe(
            flatMap(() => this.storeService.getJobInfo(job)),
            tap(jobInfo => console.log('jobInfo', jobInfo)),
            publishReplay(),
            refCount(),
          );
          return {
            id: job,
            name: job$.pipe(map(j => j.name), distinctUntilChanged()),
            app: job$.pipe(
              map(j => j.name),
              distinctUntilChanged(),
              flatMap(name => this.appService.getApp(name)),
            ),
            type: job$.pipe(map(j => j.type), distinctUntilChanged()),
            status: job$.pipe(map(j => j.status), distinctUntilChanged()),
            speed: job$.pipe(map(j => j.speed), distinctUntilChanged()),
            progress: job$.pipe(map(j => j.progress), distinctUntilChanged()),
            cancelable: job$.pipe(
              map(j => ({
                cancelable: j.cancelable,
              })),
              distinctUntilChanged(),
            ),
            downloadSize: job$.pipe(
              map(j => ({
                size: j.downloadSize,
              })),
              distinctUntilChanged(),
            ),
          };
        });
      }),
    );
  }
}

interface JonInfoRx {
  id: string;
  name: Observable<string>;
  type: Observable<StoreJobType>;
  status: Observable<StoreJobStatus>;
  speed: Observable<number>;
  progress: Observable<number>;
  cancelable: Observable<{ cancelable: boolean }>;
  downloadSize: Observable<{ size: number }>;
}
