import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable, timer, of, iif, forkJoin } from 'rxjs';
import { flatMap, map, tap, share, shareReplay } from 'rxjs/operators';
import * as ScrollIntoView from 'scroll-into-view/scrollIntoView';

import { App, AppService } from '../../services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { CanvasUtil } from '../../utils/canvas-util';
import { StoreService } from '../../dstore-client.module/services/store.service';
import { AppJobStatus, StoreJobInfo } from '../../dstore-client.module/models/store-job-info';
import { ReminderService } from '../../services/reminder.service';
import { DownloadService } from '../../services/download.service';
import { NotifyService } from '../../services/notify.service';
import { NotifyType, NotifyStatus } from '../../services/notify.model';

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

  metadataServer = BaseService.serverHosts.metadataServer;
  open = this.storeService.openApp;

  AppJobStatus = AppJobStatus;
  job$: Observable<StoreJobInfo>;
  size$: Observable<number>;
  app$: Observable<App>;

  ngOnInit() {
    this.app$ = this.route.paramMap.pipe(
      flatMap(param => {
        const appName = param.get('appName');
        return this.appService.getApp(appName);
      }),
    );
    this.job$ = timer(0, 1000).pipe(
      flatMap(() => this.app$),
      flatMap(app => this.storeService.getJobByName(app.name)),
    );
    this.size$ = this.app$.pipe(flatMap(app => this.storeService.appDownloadSize(app.name)));
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
        () => {
          this.notifyService.error(NotifyType.Reminder);
        },
      );
  }
}
