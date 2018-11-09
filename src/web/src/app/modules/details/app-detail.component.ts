import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { DomSanitizer } from '@angular/platform-browser';
import { ActivatedRoute } from '@angular/router';
import { Observable, timer, of, iif, forkJoin, merge, combineLatest } from 'rxjs';
import { flatMap, map, tap, concat, switchMap, startWith } from 'rxjs/operators';

import { App, AppService } from 'app/services/app.service';
import { BaseService } from 'app/dstore/services/base.service';
import { CanvasUtil } from 'app/utils/canvas-util';
import { StoreService } from 'app/dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
} from 'app/dstore-client.module/models/store-job-info';
import { ReminderService } from 'app/services/reminder.service';
import { DownloadService } from 'app/services/download.service';
import { NotifyService } from 'app/services/notify.service';
import { NotifyType, NotifyStatus } from 'app/services/notify.model';
import { AppVersion } from 'app/dstore-client.module/models/app-version';
import { DstoreObject } from 'app/dstore-client.module/utils/dstore-objects';

@Component({
  selector: 'app-app-detail',
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
  ) {}
  metadataServer = BaseService.serverHosts.metadataServer;
  adVisible = DstoreObject.AdVisible();
  open = this.storeService.openApp;

  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  app: App = null;
  size: number = null;
  job$: Observable<StoreJobInfo>;

  openURL = DstoreObject.openURL;
  pause = this.storeService.pauseJob;
  start = this.storeService.resumeJob;
  ngOnInit() {
    this.route.paramMap
      .pipe(switchMap(param => this.appService.getApp(param.get('appName'))))
      .subscribe(app => {
        this.app = app;
        this.job$ = merge(this.storeService.getJobList(), this.storeService.jobListChange()).pipe(
          tap(() => {
            this.storeService.getVersion([app.name]).subscribe(v => (this.app.version = v[0]));
            this.storeService.appDownloadSize(app.name).subscribe(size => (this.size = size));
          }),
          switchMap(jobs => this.storeService.getJobsInfo(jobs)),
          map(jobs => jobs.find(job => job.names.includes(app.name))),
          switchMap(job =>
            !job
              ? of(undefined)
              : timer(1000, 1000).pipe(
                  switchMap(() => this.storeService.getJobInfo(job.job)),
                  startWith(job),
                ),
          ),
          tap(job => console.log(job)),
        );
      });
  }

  reminder() {
    this.reminderService.reminder(this.app.name, this.app.version.remoteVersion).subscribe(
      () => {
        this.notifyService.success(NotifyType.Reminder);
      },
      err => {
        this.notifyService.error(NotifyType.Reminder);
      },
    );
  }

  // Show 'open' button only if app open method is 'desktop'.
  appOpenable(app: App): boolean {
    return app.extra.open === 'desktop';
  }
}
