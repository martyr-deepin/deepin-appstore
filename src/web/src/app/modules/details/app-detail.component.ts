import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { publishReplay, refCount, switchMap, share } from 'rxjs/operators';

import { StoreService } from 'app/modules/client/services/store.service';
import { StoreJobType, StoreJobStatus } from 'app/modules/client/models/store-job-info';
import { ReminderService } from 'app/services/reminder.service';
import { NotifyService } from 'app/services/notify.service';
import { NotifyType } from 'app/services/notify.model';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { environment } from 'environments/environment';
import { SoftwareService, Source } from 'app/services/software.service';
import { SettingService } from 'app/services/settings.service';

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
    private settingService: SettingService,
  ) {}

  supportSignIn = environment.supportSignIn;
  adVisible$ = this.settingService.settings$.then(set => set.upyunBannerVisible);
  open = this.softwareService.open;

  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;
  SoftSource = Source;

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
  reminder(name: string, version: string) {
    this.reminderService.reminder(name, version).subscribe(
      () => {
        this.notifyService.success(NotifyType.Reminder);
      },
      () => {
        this.notifyService.error(NotifyType.Reminder);
      },
    );
  }
}
