import { Component, OnInit, Input, OnChanges, EventEmitter, Output } from '@angular/core';
import { Observable, timer, of, empty, forkJoin, merge, combineLatest, from } from 'rxjs';
import {
  map,
  tap,
  flatMap,
  shareReplay,
  switchMap,
  debounceTime,
  concat,
  concatMap,
  startWith,
  scan,
} from 'rxjs/operators';
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
import { AppService } from '../../services/app.service';

@Component({
  selector: 'app-app-list',
  templateUrl: './app-list.component.html',
  styleUrls: ['./app-list.component.scss'],
})
export class AppListComponent implements OnInit, OnChanges {
  constructor(private appService: AppService, private storeService: StoreService) {}
  server = BaseService.serverHosts.metadataServer;
  appJobStatus = AppJobStatus;

  @Input() apps$: Observable<App[]>;
  @Input() sortBy: SortOrder;
  @Input() maxCount: number;
  @Input() rankIndex: boolean;
  @Input() subtitle = 'category';
  @Output() appListLength = new EventEmitter<number>(true);
  appList$: Observable<App[]>;
  appJobMap$: Observable<{ [key: string]: Observable<StoreJobInfo> }>;
  appVersionMap$: Observable<{ [key: string]: AppVersion }>;
  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;

  openApp = this.storeService.openApp;
  installApp = (appName: string) => this.storeService.installPackage(appName).subscribe();
  updateApp = (appName: string) => this.storeService.updatePackage(appName).subscribe();

  ngOnInit() {}

  ngOnChanges() {
    if (!this.apps$) {
      return;
    }
    this.storeService.appDownloadSize('gedit').subscribe();
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
    this.appJobMap$ = merge(this.storeService.getJobList(), this.storeService.jobListChange()).pipe(
      switchMap(jobs => this.storeService.getJobsInfo(jobs)),
      map(jobs => {
        return jobs.reduce<{ [key: string]: Observable<StoreJobInfo> }>(
          (obj, job) =>
            Object.assign(obj, {
              [job.name]: timer(0, 1000).pipe(
                switchMap(() => this.storeService.getJobInfo(job.job)),
              ),
            }),
          {},
        );
      }),
    );
    this.appVersionMap$ = combineLatest(
      this.storeService.jobListChange().pipe(startWith([])),
      this.appList$,
      (job, apps) => apps,
    ).pipe(
      switchMap(apps => this.storeService.getVersion(apps.map(app => app.name))),
      map(versionList => {
        return versionList.reduce<{ [key: string]: AppVersion }>(
          (obj, v) => Object.assign(obj, { [v.name]: v }),
          {},
        );
      }),
    );
  }
}
