import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs/Observable';

import { memoize, throttle } from 'lodash';

import { App } from '../../dstore/services/app';
import { AppService } from '../../dstore/services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { StoreService } from '../../services/store.service';
import { StoreJobInfo } from '../../services/store-job-info';

@Component({
  selector: 'app-download',
  templateUrl: './download-manage.component.html',
  styleUrls: ['./download-manage.component.scss'],
})
export class DownloadComponent implements OnInit {
  metadataServer = BaseService.serverHosts.metadataServer;
  constructor(private appService: AppService, private storeService: StoreService) {}

  jobs$: Observable<JobInfo[]>;
  progressMessage = progressMessage;
  getSizeCache = memoize((name: string) => this.storeService.appDownloadSize(name).shareReplay());

  // 下载任务控制
  start = throttle(this.storeService.resumeJob, 1000);
  pause = throttle(this.storeService.pauseJob, 1000);
  cancel = throttle(this.storeService.clearJob, 1000);

  ngOnInit() {
    this.jobs$ = Observable.timer(0, 1000)
      .mergeMap(() => this.storeService.getJobList())
      .mergeMap(jobs => {
        return Observable.forkJoin(...jobs.map(job => this.storeService.getJobInfo(job)));
      })
      .map((jobInfoList: JobInfo[]) => {
        console.log(jobInfoList);
        jobInfoList.forEach(job => (job.size$ = this.getSizeCache(job.name)));
        return jobInfoList;
      });
  }
}

class JobInfo extends StoreJobInfo {
  size$: Observable<number>;
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
