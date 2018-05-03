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
  pause = throttle(this.storeService.pauseJob, 1000);
  cancel = throttle(this.storeService.clearJob, 1000);
  start = throttle(this.storeService.resumeJob, 1000);
  getSizeCache = memoize((name: string) => this.storeService.appDownloadSize(name).shareReplay());
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
