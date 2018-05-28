import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable, timer, of, iif, forkJoin, merge, combineLatest } from 'rxjs';
import { flatMap, map, tap, concat, switchMap, publishReplay, refCount } from 'rxjs/operators';
import * as ScrollIntoView from 'scroll-into-view/scrollIntoView';

import { App, AppService } from '../../services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { CanvasUtil } from '../../utils/canvas-util';
import { StoreService } from '../../dstore-client.module/services/store.service';
import {
  AppJobStatus,
  StoreJobInfo,
  StoreJobType,
} from '../../dstore-client.module/models/store-job-info';
import { ReminderService } from '../../services/reminder.service';
import { DownloadService } from '../../services/download.service';
import { NotifyService } from '../../services/notify.service';
import { NotifyType, NotifyStatus } from '../../services/notify.model';
import { AppVersion } from '../../dstore-client.module/models/app-version';

@Component({
  selector: 'app-app-detail',
  templateUrl: './app-detail.component.html',
  styleUrls: ['./app-detail.component.scss'],
})
export class AppDetailComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private appService: AppService,
    private storeService: StoreService,
    private reminderService: ReminderService,
    private downloadService: DownloadService,
    private notifyService: NotifyService,
  ) {}
  appName: string;
  metadataServer = BaseService.serverHosts.metadataServer;
  open = this.storeService.openApp;

  AppJobStatus = AppJobStatus;
  StoreJobType = StoreJobType;
  job$: Observable<StoreJobInfo>;
  size$: Observable<number>;
  app$: Observable<App>;
  version$: Observable<AppVersion>;
  @ViewChild('$donate') donate: ElementRef<HTMLDialogElement>;

  ngOnInit() {
    this.app$ = this.route.paramMap.pipe(
      flatMap(param => this.appService.getApp(param.get('appName'))),
      publishReplay(),
      refCount(),
    );
    this.job$ = this.app$.pipe(
      flatMap(
        app =>
          merge(this.storeService.getJobList(), this.storeService.jobListChange()).pipe(
            flatMap(
              jobs =>
                jobs.length === 0
                  ? of([] as StoreJobInfo[])
                  : forkJoin(jobs.map(job => this.storeService.getJobInfo(job))),
            ),
          ),
        (app, jobs) => jobs.find(job => job.name === app.name),
      ),
    );
    this.version$ = this.app$.pipe(
      flatMap(app =>
        merge(
          of(app.version),
          this.storeService.jobListChange().pipe(
            switchMap(() => this.storeService.getVersion([app.name])),
            map(v => v[0]),
          ),
        ),
      ),
    );
    this.size$ = merge(of(null), this.storeService.jobListChange()).pipe(
      switchMap(() => this.app$),
      switchMap(app => this.storeService.appDownloadSize(app.name)),
    );
  }

  install(appName: string) {
    this.storeService
      .installPackage(appName)
      .pipe(flatMap(() => this.downloadService.record(appName)))
      .subscribe();
  }

  screenshotClick(elID: string) {
    // chrome 61 support
    // const opt: ScrollIntoViewOptions = {
    //   behavior: 'smooth',
    //   inline: 'center',
    //   block: 'nearest',
    // };
    // (<HTMLElement>event.target).scrollIntoView(opt);

    ScrollIntoView(document.getElementById(elID));
  }

  previewImage(img: HTMLImageElement) {
    window['dstore'].channel.objects.imageViewer.openBase64(CanvasUtil.getBase64Image(img));
  }

  reminder() {
    this.app$
      .pipe(flatMap(app => this.reminderService.reminder(app.name, app.version.remoteVersion)))
      .subscribe(
        () => {
          this.notifyService.success(NotifyType.Reminder);
        },
        err => {
          this.notifyService.error(NotifyType.Recommend);
        },
      );
  }
  donateOpen() {
    this.donate.nativeElement.showModal();
  }
  dialogClick(el: HTMLElement) {
    if (el.nodeName === 'DIALOG') {
      this.donate.nativeElement.close();
    }
  }
}
