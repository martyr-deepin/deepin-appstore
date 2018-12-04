import { AppSource } from './../../dstore/services/source';
import { Component, OnInit, ViewChild, ElementRef, OnDestroy } from '@angular/core';
import { DomSanitizer } from '@angular/platform-browser';
import { ActivatedRoute } from '@angular/router';
import { Observable, timer, of, iif, forkJoin, merge, combineLatest } from 'rxjs';
import { flatMap, publishReplay, tap, publish, refCount, switchMap, share } from 'rxjs/operators';

import { App, AppService } from 'app/services/app.service';
import { BaseService } from 'app/dstore/services/base.service';
import { CanvasUtil } from 'app/utils/canvas-util';
import { StoreService } from 'app/modules/client/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
} from 'app/modules/client/models/store-job-info';
import { ReminderService } from 'app/services/reminder.service';
import { DownloadService } from 'app/services/download.service';
import { NotifyService } from 'app/services/notify.service';
import { NotifyType, NotifyStatus } from 'app/services/notify.model';
import { AppVersion } from 'app/modules/client/models/app-version';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { JobService } from 'app/services/job.service';

@Component({
  selector: 'dstore-app-detail',
  templateUrl: './app-detail.component.html',
  styleUrls: ['./app-detail.component.scss'],
})
export class AppDetailComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private sanitizer: DomSanitizer,
    private appService: AppService,
    private storeService: StoreService,
    private reminderService: ReminderService,
    private downloadService: DownloadService,
    private notifyService: NotifyService,
    private jobService: JobService,
  ) {}
  metadataServer = BaseService.serverHosts.metadataServer;
  adVisible = DstoreObject.AdVisible();
  open = this.storeService.openApp;

  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;
  AppSource = AppSource;

  openURL = DstoreObject.openURL;
  pause = this.storeService.pauseJob;
  start = this.storeService.resumeJob;

  app$ = this.route.paramMap.pipe(
    tap(() => console.log('test')),
    switchMap(param => this.appService.getApp(param.get('appName'))),
    publishReplay(1),
    refCount(),
  );
  size$ = this.app$.pipe(
    switchMap(app => this.storeService.queryDownloadSize([app])),
    share(),
  );
  job$ = this.app$.pipe(
    switchMap(
      () => this.jobService.jobsInfo(),
      (app, jobs) => {
        return jobs.find(job => job.names.includes(app.name));
      },
    ),
  );
  ngOnInit() {}

  reminder(app: App) {
    this.reminderService.reminder(app.name, app.version.remoteVersion).subscribe(
      () => {
        this.notifyService.success(NotifyType.Reminder);
      },
      () => {
        this.notifyService.error(NotifyType.Reminder);
      },
    );
  }

  // Show 'open' button only if app open method is 'desktop'.
  appOpenable(app: App): boolean {
    return app.extra.open === 'desktop';
  }
}
