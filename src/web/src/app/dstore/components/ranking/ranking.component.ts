import { Component, OnInit, Input, Output, OnDestroy, EventEmitter } from '@angular/core';

import { Observable, Subscription, merge, timer, of } from 'rxjs';
import { map, tap, flatMap, shareReplay, switchMap, concat, startWith } from 'rxjs/operators';

import { BaseService } from '../../services/base.service';
import { App, AppService } from 'app/services/app.service';

import { Section } from '../../services/section';
import { AppFilterFunc, Allowed } from '../appFilter';
import { StoreService } from 'app/modules/client/services/store.service';
import { StoreJobInfo, StoreJobStatus, StoreJobType } from 'app/modules/client/models/store-job-info';
import { AppVersion } from 'app/modules/client/models/app-version';
import { JobService } from 'app/services/job.service';

@Component({
  selector: 'dstore-ranking',
  templateUrl: './ranking.component.html',
  styleUrls: ['./ranking.component.scss'],
})
export class RankingComponent implements OnInit, OnDestroy {
  constructor(private appService: AppService, private storeService: StoreService, private jobService: JobService) {}

  metadataServer = BaseService.serverHosts.metadataServer;
  isNative = BaseService.isNative;
  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;
  @Output()
  loaded = new EventEmitter<Boolean>();
  @Input()
  section: Section;
  @Input()
  appFilter: AppFilterFunc = Allowed;

  apps$: Observable<App[]>;
  jobs: { [key: string]: StoreJobInfo } = {};
  jobsNames = new Set<string>();
  jobs$: Subscription;
  version$: Subscription;

  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  openApp = this.storeService.openApp;

  ngOnInit() {
    const category = this.section.ranking.category;
    this.getJobs();
    this.apps$ = this.appService.list().pipe(
      map(apps => {
        if (category) {
          apps = apps.filter(app => app.category === category);
        }
        return apps
          .sort((a, b) => b.downloads - a.downloads)
          .slice(0, this.section.ranking.count + 10)
          .map(app => app.name);
      }),
      switchMap(appNameList => this.appService.getApps(appNameList)),
      tap(() => this.loaded.emit(true)),
    );
  }

  getJobs() {
    this.jobs$ = this.jobService.jobsInfo().subscribe(jobInfos => {
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
