import { Component, OnInit, OnDestroy } from '@angular/core';
import { DomSanitizer } from '@angular/platform-browser';
import { trigger, state, style, animate, transition, query } from '@angular/animations';

import { Observable, timer, merge, forkJoin, of, Subscription } from 'rxjs';
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
export class UninstallComponent implements OnInit, OnDestroy {
  constructor(
    private sanitizer: DomSanitizer,
    private storeService: StoreService,
    private appService: AppService,
  ) {}
  disabledList = [
    'dde-control-center',
    'dde-introduction',
    'dde-file-manager',
    'deepin-appstore',
    'deepin-manual',
  ];
  installedApps: InstalledApp[];
  uninstallingApps: string[];
  select = '';
  job: Subscription;

  ngOnInit() {
    this.job = merge(this.storeService.getJobList(), this.storeService.jobListChange())
      .pipe(
        tap(() => {
          this.storeService.getInstalledApps().subscribe(list => {
            this.installedApps = _.sortBy(list, 'time').reverse();
          });
        }),
        switchMap(
          jobs =>
            jobs.length > 0
              ? timer(1000, 1000).pipe(flatMap(() => this.storeService.getJobsInfo(jobs)))
              : of([]),
        ),
      )
      .subscribe(jobInfoList => {
        console.log(jobInfoList);
        this.uninstallingApps = [].concat(
          ...jobInfoList
            .filter(
              job => job.type === StoreJobType.uninstall && job.status !== StoreJobStatus.failed,
            )
            .map(job => job.names),
        );
      });
  }
  ngOnDestroy() {
    this.job.unsubscribe();
  }

  uninstall(appName: string, localName: string) {
    this.storeService.removePackage(appName, localName).subscribe(() => {
      this.select = '';
      this.uninstallingApps.push(appName);
    });
  }
}
