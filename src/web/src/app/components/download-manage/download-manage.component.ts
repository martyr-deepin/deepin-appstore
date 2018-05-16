import { Component, OnInit } from '@angular/core';
import { Observable, of, forkJoin, timer, iif } from 'rxjs';
import { flatMap, defaultIfEmpty, map, tap, shareReplay } from 'rxjs/operators';

import { memoize, throttle } from 'lodash';

import { App } from '../../dstore/services/app';
import { AppService } from '../../dstore/services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { StoreService } from '../../services/store.service';
import { StoreJobInfo, StoreJobType, StoreJobStatus } from '../../services/store-job-info';
import { AppVersion } from '../../services/app-version';

@Component({
  selector: 'app-download',
  templateUrl: './download-manage.component.html',
  styleUrls: ['./download-manage.component.scss'],
})
export class DownloadComponent implements OnInit {
  metadataServer = BaseService.serverHosts.metadataServer;
  constructor(private appService: AppService, private storeService: StoreService) {}

  StoreJobType = StoreJobType;
  StoreJobStatus = StoreJobStatus;

  // 下载任务控制
  start = throttle(this.storeService.resumeJob, 1000);
  pause = throttle(this.storeService.pauseJob, 1000);
  cancel = throttle(this.storeService.clearJob, 1000);

  jobs$: Observable<StoreJobInfo[]>;
  version$: Observable<AppVersion>;
  ngOnInit() {
    this.jobs$ = timer(0, 1000).pipe(
      flatMap(() => this.storeService.getJobList()),
      flatMap(jobs =>
        iif(
          () => jobs.length === 0,
          of([] as StoreJobInfo[]),
          forkJoin(jobs.map(job => this.storeService.getJobInfo(job))),
        ),
      ),
      map(jobs => jobs.filter(job => job.type !== 'remove')),
    );
  }
}

const progressMessage = {
  download: {
    paused: '已暂停',
    running: '正在下载',
    ready: '等待下载',
    failed: '下载失败',
  },
  install: {
    running: '正在安装',
    ready: '等待安装',
    failed: '安装失败',
  },
};
