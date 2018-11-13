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
} from 'app/modules/client/models/store-job-info';
import { SectionAssemble, Section, SectionApp } from '../../services/section';
import { AppFilterFunc, Allowed } from '../appFilter';
import { CategoryService, Category } from '../../services/category.service';
import { StoreService } from 'app/modules/client/services/store.service';
import { AppVersion } from 'app/modules/client/models/app-version';
import { JobService } from 'app/services/job.service';

@Component({
  selector: 'dstore-assemble',
  templateUrl: './assemble.component.html',
  styleUrls: ['./assemble.component.scss'],
})
export class AssembleComponent implements OnInit, OnDestroy {
  server = BaseService.serverHosts.metadataServer;
  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  @Input()
  section: Section;
  @Input()
  assembleList: SectionAssemble[] = [];
  @Input()
  appFilter: AppFilterFunc = Allowed;

  constructor(
    private appService: AppService,
    private sanitizer: DomSanitizer,
    private category: CategoryService,
    private storeService: StoreService,
    private jobService: JobService,
  ) {}
  categoryList: { [key: string]: Category };

  // data
  versions = new Map<string, AppVersion>();
  jobs: { [key: string]: StoreJobInfo } = {};
  jobsNames = new Set<string>();
  jobs$: Subscription;
  version$: Subscription;

  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  openApp = this.storeService.openApp;

  ngOnInit() {
    this.category.getList().subscribe(cs => {
      this.categoryList = cs;
    });
    this.jobs$ = this.getJobs();
    this.version$ = this.getVersions();
  }
  ngOnDestroy() {
    if (this.jobs$) {
      this.jobs$.unsubscribe();
    }
    if (this.version$) {
      this.version$.unsubscribe();
    }
  }

  async filter(apps: SectionApp[]) {
    const versionMap = await this.storeService.getVersionMap(apps.map(app => app.name)).toPromise();
    return apps.filter(app => versionMap.has(app.name));
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

  getVersions() {
    const apps = [].concat(
      ...this.assembleList.map(assemble =>
        assemble.apps.filter(app => app.show).map(app => app.name),
      ),
    );
    return this.jobService
      .jobList()
      .pipe(
        flatMap(() => {
          return this.storeService.getVersion(apps);
        }),
      )
      .subscribe(vs => {
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
