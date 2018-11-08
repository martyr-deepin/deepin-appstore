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
import { JobService } from 'app/services/job.service';

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
    private jobService: JobService,
  ) {}
  disabledList = [
    'dde-control-center',
    'dde-introduction',
    'dde-file-manager',
    'deepin-appstore',
    'deepin-manual',
  ];
  installedApps: InstalledApp[];
  uninstallingApps = new Set<string>();
  select = '';
  job: Subscription;

  ngOnInit() {
    this.job = this.jobService.jobsInfo().subscribe(jobs => {
      jobs
        .filter(job => job.type === StoreJobType.uninstall && job.status !== StoreJobStatus.failed)
        .forEach(job => job.names.forEach(name => this.uninstallingApps.add(name)));
    });
    this.jobService
      .jobList()
      .pipe(
        switchMap(() => {
          return this.storeService.getInstalledApps();
        }),
      )
      .subscribe(list => {
        this.installedApps = _.sortBy(list, 'time').reverse();
      });
  }
  ngOnDestroy() {
    this.job.unsubscribe();
  }

  uninstall(appName: string, localName: string) {
    this.storeService.removePackage(appName, localName).subscribe(() => {
      this.select = '';
      this.uninstallingApps.add(appName);
    });
  }
}
