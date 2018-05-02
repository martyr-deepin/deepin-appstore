import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import 'rxjs/add/operator/map';

import * as _ from 'lodash';

import { BaseService } from '../dstore/services/base.service';
import { App } from '../dstore/services/app';
import { StoreJobInfo } from './store-job-info';
import { StoreService } from './store.service';

@Injectable()
export class DownloadService {
  constructor(private storeService: StoreService) {
    this.storeService.getJobList().subscribe(job => console.log('job', job));
    // this.storeService.installPackage('0ad').subscribe(job => {
    //   console.log(job);
    //   this.storeService.pauseJob(job);
    // })
  }
}
