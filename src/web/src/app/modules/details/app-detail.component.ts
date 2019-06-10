import { AppSource } from './../../dstore/services/source';
import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { publishReplay, tap, refCount, switchMap, share } from 'rxjs/operators';

import { App, AppService } from 'app/services/app.service';
import { BaseService } from 'app/dstore/services/base.service';
import { StoreService } from 'app/modules/client/services/store.service';
import { StoreJobInfo, StoreJobType, StoreJobStatus } from 'app/modules/client/models/store-job-info';
import { ReminderService } from 'app/services/reminder.service';
import { NotifyService } from 'app/services/notify.service';
import { NotifyType } from 'app/services/notify.model';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { JobService } from 'app/services/job.service';
import { environment } from 'environments/environment';
import { SoftwareService } from 'app/services/software.service';

@Component({
  selector: 'dstore-app-detail',
  templateUrl: './app-detail.component.html',
  styleUrls: ['./app-detail.component.scss'],
})
export class AppDetailComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private softwareService: SoftwareService,
    private storeService: StoreService,
    private reminderService: ReminderService,
    private notifyService: NotifyService,
  ) {}
  supportSignIn = environment.supportSignIn;
  adVisible = DstoreObject.AdVisible();
  open = this.storeService.openApp;

  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;
  AppSource = AppSource;

  openURL = DstoreObject.openURL;
  pause = this.storeService.pauseJob;
  start = this.storeService.resumeJob;

  app$ = this.route.paramMap.pipe(
    switchMap(param => this.softwareService.list({ names: [param.get('appName')] }).then(softs => softs[0])),
    publishReplay(1),
    refCount(),
  );
  size$ = this.app$.pipe(
    switchMap(app => this.softwareService.size(app)),
    share(),
  );
  allowName$ = this.storeService.getAllowShowPackageName();

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
}
