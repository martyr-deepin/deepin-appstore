import { Component, OnInit, OnDestroy } from '@angular/core';
import { trigger, state, style, animate, transition } from '@angular/animations';
import { Observable, of, forkJoin, timer, iif, merge, Subscription } from 'rxjs';
import { switchMap, map, tap } from 'rxjs/operators';

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
import { DomSanitizer } from '@angular/platform-browser';

@Component({
  selector: 'app-download',
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
  metadataServer = BaseService.serverHosts.metadataServer;
  constructor(
    private appService: AppService,
    private storeService: StoreService,
    private sanitizer: DomSanitizer,
  ) {}

  StoreJobType = StoreJobType;
  StoreJobStatus = StoreJobStatus;

  // 下载任务控制
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  cancel = this.storeService.clearJob;

  jobs: StoreJobInfo[] = [];
  jobs$: Subscription;
  ngOnInit() {
    this.jobs$ = merge(this.storeService.getJobList(), this.storeService.jobListChange())
      .pipe(
        map(jobs => jobs.filter(job => job.includes('install')).sort((a, b) => b.localeCompare(a))),
        tap(jobs => {
          this.jobs.forEach((job, index) => {
            if (!jobs.includes(job.job)) {
              console.log('remove', job);
              this.jobs.splice(index, 1);
            }
          });
        }),
        switchMap(jobs => timer(0, 1000), jobs => jobs),
        switchMap(jobs => this.storeService.getJobsInfo(jobs)),
      )
      .subscribe(jobInfos => {
        jobInfos.sort((a, b) => a.createTime - b.createTime).forEach(jobInfo => {
          const oldJob = this.jobs.find(job => job.id === jobInfo.id);
          if (oldJob) {
            Object.assign(oldJob, jobInfo);
          } else {
            this.jobs.push(jobInfo);
          }
        });
      });
  }
  ngOnDestroy() {
    this.jobs$.unsubscribe();
  }
  getUrl(app: App) {
    if (navigator.onLine) {
      return this.metadataServer + '/' + app.icon;
    }
    return this.sanitizer.bypassSecurityTrustUrl('rcc://icon/' + app.name);
  }
}
