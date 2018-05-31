import { Component, OnInit } from '@angular/core';
import { trigger, state, style, animate, transition, query } from '@angular/animations';

import { Observable, timer, merge, forkJoin, of } from 'rxjs';
import { map, flatMap, switchMap, tap } from 'rxjs/operators';

import * as _ from 'lodash';

import { StoreService } from '../../../dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  StoreJobStatus,
  StoreJobType,
  AppJobStatus,
} from '../../../dstore-client.module/models/store-job-info';
import { AppService, App } from '../../../services/app.service';
import { BaseService } from '../../../dstore/services/base.service';
import { InstalledApp } from '../../../dstore-client.module/models/installed';

@Component({
  selector: 'app-uninstall',
  templateUrl: './uninstall.component.html',
  styleUrls: ['./uninstall.component.scss'],
})
export class UninstallComponent implements OnInit {
  constructor(private storeService: StoreService, private appService: AppService) {}
  metadataServer = BaseService.serverHosts.metadataServer;

  installedApps$: Observable<InstalledApp[]>;
  uninstallingApps$: Observable<string[]>;
  ngOnInit() {
    this.uninstallingApps$ = merge(
      this.storeService.getJobList(),
      this.storeService.jobListChange(),
    ).pipe(
      switchMap(
        jobList =>
          jobList.length === 0
            ? of([])
            : forkJoin(jobList.map(job => this.storeService.getJobInfo(job))),
      ),
      map(jobInfoList => {
        return jobInfoList
          .filter(
            jobInfo =>
              jobInfo.type === StoreJobType.uninstall && jobInfo.status !== StoreJobStatus.failed,
          )
          .map(jobInfo => jobInfo.name);
      }),
    );
    this.installedApps$ = this.uninstallingApps$.pipe(
      switchMap(() => this.storeService.getInstalledApps()),
      map(list => _.sortBy(list, 'time').reverse()),
    );
  }

  uninstall(appName: string) {
    this.storeService.removePackage(appName).subscribe();
  }
}
