import { Component, OnInit, Input, OnChanges, EventEmitter, Output } from '@angular/core';
import { Observable, timer, of, empty, forkJoin, merge } from 'rxjs';
import { map, tap, flatMap, shareReplay } from 'rxjs/operators';
import * as _ from 'lodash';
import { sortBy } from 'lodash';

import { App } from '../../services/app.service';
import { SortOrder } from '../app-title/app-title.component';
import { BaseService } from '../../dstore/services/base.service';
import { StoreService } from '../../dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  AppJobInfo,
  AppJobStatus,
} from '../../dstore-client.module/models/store-job-info';
import { AppVersion } from '../../dstore-client.module/models/app-version';

@Component({
  selector: 'app-app-list',
  templateUrl: './app-list.component.html',
  styleUrls: ['./app-list.component.scss'],
})
export class AppListComponent implements OnInit, OnChanges {
  constructor(private storeService: StoreService) {}
  server = BaseService.serverHosts.metadataServer;
  appJobStatus = AppJobStatus;

  @Input() apps$: Observable<App[]>;
  @Input() sortBy: SortOrder;
  @Input() maxCount: number;
  @Output() appListLength = new EventEmitter<number>(true);
  appList$: Observable<App[]>;
  // job control
  start = _.throttle(this.storeService.resumeJob, 1000);
  pause = _.throttle(this.storeService.pauseJob, 1000);
  cancel = _.throttle(this.storeService.clearJob, 1000);

  getAppJob = _.memoize((appName: string): Observable<StoreJobInfo> => {
    return timer(0, 1000).pipe(flatMap(() => this.storeService.getJobByName(appName)));
  });
  getAppVersion = _.memoize((appName: string): Observable<AppVersion> => {
    return timer(0, 1000).pipe(
      flatMap(() => this.storeService.getVersion([appName])),
      map(vs => vs[0]),
    );
  });

  openApp = this.storeService.openApp;
  installApp = (appName: string) => this.storeService.installPackage(appName).subscribe();
  updateApp = (appName: string) => this.storeService.updatePackage(appName).subscribe();

  ngOnInit() {}

  ngOnChanges() {
    if (!this.apps$) {
      return;
    }
    this.appList$ = this.apps$.pipe(
      map(apps => {
        apps = apps.filter(app => app);
        if (this.sortBy) {
          apps = sortBy(apps, [
            this.sortBy === SortOrder.Downloads ? 'downloads' : 'rate',
            'name',
          ]).reverse();
        }
        if (this.maxCount) {
          apps = apps.slice(0, this.maxCount);
        }
        return apps;
      }),
      tap(apps => {
        this.appListLength.emit(apps.length);
      }),
    );
  }
}
