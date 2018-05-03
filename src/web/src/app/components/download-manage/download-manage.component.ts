import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs/Observable';

import { memoize } from 'lodash';

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
  getSizeCache = memoize((name: string) => {
    console.log('getSize');
    return this.storeService.appDownloadSize(name).shareReplay();
  });
  ngOnInit() {
    console.log(this.metadataServer);

    this.jobs$ = Observable.timer(0, 3000)
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
