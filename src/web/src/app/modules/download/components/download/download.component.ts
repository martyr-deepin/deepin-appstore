import { Component, OnInit, OnDestroy } from '@angular/core';
import {
  trigger,
  state,
  style,
  animate,
  transition,
} from '@angular/animations';
import {
  Observable,
  of,
  forkJoin,
  timer,
  iif,
  merge,
  Subscription,
} from 'rxjs';
import {
  switchMap,
  map,
  tap,
  filter,
  publishReplay,
  refCount,
} from 'rxjs/operators';

import { AppService, App } from 'app/services/app.service';
import { StoreService } from 'app/modules/client/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
  CanFixError,
  StoreJobError,
  StoreJobErrorType,
} from 'app/modules/client//models/store-job-info';
import { JobService } from 'app/services/job.service';

@Component({
  selector: 'dstore-download',
  templateUrl: './download.component.html',
  styleUrls: ['./download.component.scss'],
  animations: [
    trigger('flyInOut', [
      state('in', style({ height: '*' })),
      transition(':enter', [style({ height: 0 }), animate(100)]),
      transition(':leave', [animate(100, style({ height: 0 }))]),
    ]),
  ],
})
export class DownloadComponent implements OnInit, OnDestroy {
  constructor(
    private appService: AppService,
    private storeService: StoreService,
    private jobService: JobService,
  ) {}

  StoreJobType = StoreJobType;
  StoreJobStatus = StoreJobStatus;
  StoreJobErrorType = StoreJobErrorType;

  // 下载任务控制
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;

  loaded = false;
  result$ = this.jobService.jobsInfo().pipe(
    map(jobs => {
      return jobs
        .filter(
          job =>
            job.type === StoreJobType.download ||
            job.type === StoreJobType.install,
        )
        .sort((a, b) => b.createTime - a.createTime);
    }),
    switchMap(
      jobs => {
        const names = [].concat(...jobs.map(job => job.names));
        return this.appService.getApps(names);
      },
      (jobs, apps) => {
        return apps.map(app => ({
          app,
          job: jobs.find(job => job.names.includes(app.name)),
        }));
      },
    ),
  );

  apps = new Map<string, App>();
  jobs: StoreJobInfo[] = [];
  cancels = new Set<string>();
  fixing = false;

  ngOnInit() {}
  ngOnDestroy() {}

  retry(job: StoreJobInfo) {
    let err: StoreJobError;
    try {
      err = JSON.parse(job.description) as StoreJobError;
    } catch (e) {
      err = { ErrType: StoreJobErrorType.unknown, ErrDetail: job.description };
    }

    this.storeService.resumeJob(job.job);

    if (CanFixError.includes(err.ErrType)) {
      this.fixing = true;
      this.storeService
        .fixError(err.ErrType.toString().split('::')[1])
        .pipe(
          switchMap(
            () => this.storeService.jobListChange(),
            (jobPath, jobList) => jobList.includes(jobPath),
          ),
          filter(exists => !exists),
        )
        .subscribe(() => {
          this.fixing = false;
          this.storeService.resumeJob(job.job);
        });
    }
  }
  cancel(job: string) {
    this.cancels.add(job);
    this.storeService.clearJob(job);
  }
  floor(n: number): number {
    return Math.floor(n);
  }
  trackByFn(index, item) {
    return item.job.job;
  }
}
