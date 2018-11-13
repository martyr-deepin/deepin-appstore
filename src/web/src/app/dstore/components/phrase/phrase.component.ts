import { Component, OnInit, Input, OnDestroy } from '@angular/core';

import { Observable, Subscription, merge, timer, of } from 'rxjs';
import { map, tap, flatMap, shareReplay, switchMap, concat, startWith } from 'rxjs/operators';

import { BaseService } from '../../services/base.service';
import { AppService, App } from '../../../services/app.service';
import { SectionPhrase, Section } from '../../services/section';
import { AppFilterFunc, Allowed } from '../appFilter';
import { StoreService } from 'app/modules/client/services/store.service';
import {
  StoreJobInfo,
  StoreJobStatus,
  StoreJobType,
} from 'app/modules/client/models/store-job-info';
import { AppVersion } from 'app/modules/client/models/app-version';
import { JobService } from 'app/services/job.service';

interface AppPhrase {
  app: App;
  phrase: SectionPhrase;
}

@Component({
  selector: 'dstore-phrase',
  templateUrl: './phrase.component.html',
  styleUrls: ['./phrase.component.scss'],
})
export class PhraseComponent implements OnInit, OnDestroy {
  constructor(
    private appService: AppService,
    private storeService: StoreService,
    private jobService: JobService,
  ) {}
  // const
  metadataServer = BaseService.serverHosts.metadataServer;
  isNative = BaseService.isNative;
  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  // input
  @Input()
  section: Section;
  @Input()
  phraseList: SectionPhrase[] = [];
  @Input()
  appFilter: AppFilterFunc = Allowed;

  // data
  moreNav: any[] = [];
  appList: App[] = [];
  jobs: { [key: string]: StoreJobInfo } = {};
  jobsNames = new Set<string>();
  jobs$: Subscription;

  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  openApp = this.storeService.openApp;

  ngOnInit() {
    this.jobService
      .jobList()
      .pipe(
        flatMap(() => {
          return this.appService.getApps(
            this.phraseList.filter(app => app.show).map(app => app.name),
          );
        }),
      )
      .subscribe(appList => {
        this.appList = appList;
        this.moreNav = [
          './apps',
          { title: this.section.title, apps: appList.map(app => app.name) },
        ];
      });
    this.getJobs();
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
