import { AppSource } from './../../dstore/services/source';
import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { publishReplay, tap, refCount, switchMap, share } from 'rxjs/operators';

import { App, AppService } from 'app/services/app.service';
import { BaseService } from 'app/dstore/services/base.service';
import { StoreService } from 'app/modules/client/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
} from 'app/modules/client/models/store-job-info';
import { ReminderService } from 'app/services/reminder.service';
import { NotifyService } from 'app/services/notify.service';
import { NotifyType } from 'app/services/notify.model';
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
    private appService: AppService,
    private storeService: StoreService,
    private reminderService: ReminderService,
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
  allowName$ = this.storeService.getAllowShowPackageName();
  ngOnInit() {}

  reminder(app: App) {
    this.reminderService
      .reminder(app.name, app.version.remoteVersion)
      .subscribe(
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
