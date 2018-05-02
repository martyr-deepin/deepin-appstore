import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs/Observable';

import { App } from '../../dstore/services/app';
import { AppService } from '../../dstore/services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { StoreService } from '../../services/store.service';

@Component({
  selector: 'app-download',
  templateUrl: './download.component.html',
  styleUrls: ['./download.component.scss'],
})
export class DownloadComponent implements OnInit {
  metadataServer: string;
  constructor(private appService: AppService, private storeService: StoreService) {}
  ngOnInit() {
    Observable.timer(3000, 3000)
      .mergeMap(() => this.storeService.getJobList())
      .mergeMap(jobs => {
        console.log(jobs);
        return Observable.forkJoin(...jobs.map(job => this.storeService.getJobInfo(job)));
      })
      .subscribe(jobInfos => console.log('jobInfos', jobInfos));
  }
}
