import { Component, OnInit, Input, OnDestroy } from '@angular/core';
import { DomSanitizer } from '@angular/platform-browser';
import * as _ from 'lodash';
import { Observable, Subscription, merge, timer, of } from 'rxjs';
import { map, tap, flatMap, shareReplay, switchMap, concat, startWith } from 'rxjs/operators';

import { BaseService } from '../../services/base.service';

import { App, AppService } from '../../../services/app.service';

import {
  StoreJobInfo,
  StoreJobStatus,
  StoreJobType,
} from '../../../dstore-client.module/models/store-job-info';
import { SectionAssemble, Section, SectionApp } from '../../services/section';
import { AppFilterFunc, Allowed } from '../appFilter';
import { CategoryService, Category } from '../../services/category.service';
import { StoreService } from '../../../dstore-client.module/services/store.service';
import { AppVersion } from '../../../dstore-client.module/models/app-version';

@Component({
  selector: 'dstore-assemble',
  templateUrl: './assemble.component.html',
  styleUrls: ['./assemble.component.scss'],
})
export class AssembleComponent implements OnInit, OnDestroy {
  server = BaseService.serverHosts.metadataServer;
  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  @Input() section: Section;
  @Input() assembleList: SectionAssemble[] = [];
  @Input() appFilter: AppFilterFunc = Allowed;

  constructor(
    private appService: AppService,
    private sanitizer: DomSanitizer,
    private category: CategoryService,
    private storeService: StoreService,
  ) {}
  categoryList: { [key: string]: Category };

  // data
  versions = new Map<string, AppVersion>();
  jobs = new Map<string, StoreJobInfo>();
  jobsNames = new Set<string>();
  jobs$: Subscription;

  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  openApp = this.storeService.openApp;

  ngOnInit() {
    this.category.getList().subscribe(cs => {
      this.categoryList = cs;
    });
    this.jobs$ = this.getJobs();
  }
  ngOnDestroy() {
    this.jobs$.unsubscribe();
  }

  filter(apps: SectionApp[]) {
    return apps.filter(app => this.appFilter(app.name));
  }

  getJobs() {
    return merge(this.storeService.getJobList(), this.storeService.jobListChange())
      .pipe(
        tap(() => {
          this.getVersions();
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
        const jobs = new Map<string, StoreJobInfo>();
        jobInfos.forEach(job => {
          job.names.forEach(name => {
            jobs.set(name, job);
            this.jobsNames.add(name);
          });
        });
        this.jobs = jobs;
      });
  }

  getVersions() {
    const apps = [].concat(
      ...this.assembleList.map(assemble =>
        assemble.apps.filter(app => app.show).map(app => app.name),
      ),
    );
    this.storeService.getVersion(apps).subscribe(vs => {
      this.versions = new Map(vs.map(v => [v.name, v] as [string, AppVersion]));
    });
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
