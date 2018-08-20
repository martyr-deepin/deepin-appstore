import { Component, OnInit, Input, OnDestroy } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import * as _ from 'lodash';
import { Observable, Subscription, merge, timer, of } from 'rxjs';
import { map, tap, flatMap, shareReplay, switchMap, concat, startWith } from 'rxjs/operators';

import { BaseService } from '../../services/base.service';
import { SectionApp, Section } from '../../services/section';

import { AppFilterFunc, Allowed } from '../appFilter';

import { AppService, App } from '../../../services/app.service';
import { StoreService } from '../../../dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  StoreJobStatus,
  StoreJobType,
} from '../../../dstore-client.module/models/store-job-info';
import { AppVersion } from '../../../dstore-client.module/models/app-version';

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
  ) {}
  server = BaseService.serverHosts.metadataServer;
  isNative = BaseService.isNative;
  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  @Input() section: Section;
  @Input() apps: SectionApp[];
  @Input() appFilter: AppFilterFunc = Allowed;
  // data
  moreNav: any[];
  appList: App[] = [];
  jobs: { [key: string]: StoreJobInfo } = {};
  jobsNames = new Set<string>();
  jobs$: Subscription;

  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  openApp = this.storeService.openApp;
  ngOnInit() {
    this.appService
      .getApps(this.apps.filter(app => app.show).map(app => app.name))
      .subscribe(appList => {
        this.appList = appList;
        this.moreNav = [
          './apps',
          { title: this.section.title, apps: appList.map(app => app.name) },
        ];
        this.getJobs();
      });
  }
  getJobs() {
    this.jobs$ = merge(this.storeService.getJobList(), this.storeService.jobListChange())
      .pipe(
        tap(() => {
          this.storeService.getVersion(Object.keys(this.jobs)).subscribe(versions => {
            const vMap = new Map(versions.map(v => [v.name, v] as [string, AppVersion]));
            if (this.appList) {
              this.appList.forEach(app => {
                if (vMap.has(app.name)) {
                  app.version = vMap.get(app.name);
                }
              });
            }
          });
        }),
        switchMap(jobs => {
          if (jobs.length > 0) {
            return timer(0, 1000).pipe(flatMap(() => this.storeService.getJobsInfo(jobs)));
          } else {
            return of([] as StoreJobInfo[]);
          }
        }),
      )
      .subscribe(jobInfos => {
        const jobs: { [key: string]: StoreJobInfo } = {};
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

  installApp(app: App) {
    this.storeService.installPackage(app.name, app.localInfo.description.name).subscribe();
  }
  updateApp(app: App) {
    this.storeService.updatePackage(app.name, app.localInfo.description.name).subscribe();
  }

  // Show 'open' button only if app open method is 'desktop'.
  appOpenable(app: App): boolean {
    return app.extra.open === 'desktop';
  }
}
