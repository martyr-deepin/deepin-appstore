import { Component, OnInit, Input, Output, OnDestroy, EventEmitter } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import * as _ from 'lodash';
import { Observable, Subscription, merge, timer, of } from 'rxjs';
import { map, tap, flatMap, shareReplay, switchMap, concat, startWith } from 'rxjs/operators';

import { BaseService } from '../../services/base.service';
import { SectionApp, Section } from '../../services/section';

import { AppFilterFunc, Allowed } from '../appFilter';

import { AppService, App } from '../../../services/app.service';
import { StoreService } from 'app/modules/client/services/store.service';
import {
  StoreJobInfo,
  StoreJobStatus,
  StoreJobType,
} from 'app/modules/client/models/store-job-info';
import { AppVersion } from 'app/modules/client/models/app-version';
import { JobService } from 'app/services/job.service';

@Component({
  selector: 'dstore-cover',
  templateUrl: './cover.component.html',
  styleUrls: ['./cover.component.scss'],
})
export class CoverComponent implements OnInit, OnDestroy {
  constructor(
    private router: Router,
    private route: ActivatedRoute,
    private appService: AppService,
    private storeService: StoreService,
    private jobService: JobService,
  ) {}
  server = BaseService.serverHosts.metadataServer;
  isNative = BaseService.isNative;
  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  @Output()
  loaded = new EventEmitter<Boolean>();
  @Input()
  section: Section;
  @Input()
  apps: SectionApp[];
  @Input()
  appFilter: AppFilterFunc = Allowed;
  // data
  moreNav: any[];
  apps$: Observable<App[]>;
  jobs: { [key: string]: StoreJobInfo } = {};
  jobsNames = new Set<string>();
  jobs$: Subscription;

  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  openApp = this.storeService.openApp;

  ngOnInit() {
    this.getJobs();
    this.apps$ = this.appService
      .getApps(this.apps.filter(app => app.show).map(app => app.name))
      .pipe(
        tap(apps => {
          this.moreNav = ['./apps', { title: this.section.title, apps: apps.map(app => app.name) }];
          this.loaded.emit(true);
        }),
      );
  }

  getJobs() {
    return this.jobService.jobsInfo().subscribe(jobInfos => {
      const jobs = {};
      jobInfos.forEach(job => {
        job.names.forEach(name => {
          jobs[name] = job;
          this.jobsNames.add(name);
        });
      });
      this.jobs = jobs;
    });
  }

  ngOnDestroy() {
    if (this.jobs$) {
      this.jobs$.unsubscribe();
    }
  }
}
