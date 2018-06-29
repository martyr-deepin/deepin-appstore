import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable, timer, of, iif, forkJoin, merge, combineLatest } from 'rxjs';
import {
  flatMap,
  map,
  tap,
  concat,
  switchMap,
  publishReplay,
  refCount,
  startWith,
} from 'rxjs/operators';

import { App, AppService } from '../../services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { CanvasUtil } from '../../utils/canvas-util';
import { StoreService } from '../../dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
} from '../../dstore-client.module/models/store-job-info';
import { ReminderService } from '../../services/reminder.service';
import { DownloadService } from '../../services/download.service';
import { NotifyService } from '../../services/notify.service';
import { NotifyType, NotifyStatus } from '../../services/notify.model';
import { AppVersion } from '../../dstore-client.module/models/app-version';
import { DomSanitizer } from '@angular/platform-browser';

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
  open = this.storeService.openApp;

  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  app: App = null;
  size: number = null;
  job$: Observable<StoreJobInfo>;

  @ViewChild('$donate') donate: ElementRef<HTMLDialogElement>;

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
          map(jobs => jobs.find(job => job.name === app.name)),
          switchMap(
            job =>
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

  install(app: App) {
    this.storeService
      .installPackage(app.name, app.localInfo.description.name)
      .pipe(flatMap(() => this.downloadService.record(app.name)))
      .subscribe({
        complete: () => {
          app.downloads++;
        },
      });
  }

  getButtonStyle(progress: number) {
    return this.sanitizer.bypassSecurityTrustStyle(`--progress:${(progress * 100).toFixed(0)}%`);
  }

  screenshotClick(id: string) {
    const el = document.querySelector('#' + id);
    if (el) {
      const opt: ScrollIntoViewOptions = {
        behavior: 'smooth',
        inline: 'center',
        block: 'nearest',
      };
      //  chrome 61 support
      //  el.scrollIntoView(opt)
      // smoothScrollIntoView(el, opt);
      el.scrollIntoView();
    }
  }

  previewImage(img: HTMLImageElement) {
    window['dstore'].channel.objects.imageViewer.openBase64(CanvasUtil.getBase64Image(img));
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

  donateOpen() {
    this.donate.nativeElement.showModal();
  }

  dialogClick(el: HTMLElement) {
    if (el.nodeName === 'DIALOG') {
      this.donate.nativeElement.close();
    }
  }

  // Show 'open' button only if app open method is 'desktop'.
  appOpenable(app: App): boolean {
    return app.extra.open === 'desktop';
  }
}
