import { Component, OnInit, Input, Output, OnDestroy, EventEmitter } from '@angular/core';
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

  @Output()
  loaded = new EventEmitter<Boolean>();
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
  }
  ngOnDestroy() {
    if (this.jobs$) {
      this.jobs$.unsubscribe();
    }
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
}
