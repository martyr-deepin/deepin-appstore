import { map, share } from 'rxjs/operators';
import { StoreJobStatus } from './../../../client/models/store-job-info';
import {
  Component,
  OnInit,
  Input,
  OnChanges,
  EventEmitter,
  Output,
  OnDestroy,
} from '@angular/core';
import { Observable, Subscription } from 'rxjs';
import * as _ from 'lodash';
import { sortBy } from 'lodash';

import { App } from 'app/services/app.service';
import { SortOrder } from '../app-title/app-title.component';
import { BaseService } from 'app/dstore/services/base.service';
import { StoreService } from 'app/modules/client/services/store.service';
import { StoreJobInfo } from 'app/modules/client/models/store-job-info';
import { JobService } from 'app/services/job.service';

@Component({
  selector: 'app-app-list',
  templateUrl: './app-list.component.html',
  styleUrls: ['./app-list.component.scss'],
})
export class AppListComponent implements OnInit, OnChanges {
  constructor(private storeService: StoreService, private jobService: JobService) {}
  // const
  server = BaseService.serverHosts.metadataServer;
  StoreJobStatus = StoreJobStatus;

  @Input()
  appList: App[];
  @Input()
  routePrefix: string;
  @Input()
  sortBy: SortOrder;
  @Input()
  rankIndex: boolean;
  @Input()
  subtitle = 'category';
  @Output()
  appListLength = new EventEmitter<number>(true);

  apps: App[];
  jobEndList = new Set<string>();
  installApps = new Set<string>();

  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  openApp = this.storeService.openApp;

  jobs$ = this.jobService.jobsInfo().pipe(
    map(jobs => {
      const jobMap = new Map<string, StoreJobInfo>();
      jobs.forEach(job =>
        job.names.forEach(name => {
          this.jobEndList.add(name);
          jobMap.set(name, job);
        }),
      );
      return jobMap;
    }),
    share(),
  );
  ngOnInit() {
    this.changeList();
  }
  changeList() {
    if (this.appList) {
      let apps = this.appList.filter(Boolean);
      if (this.sortBy) {
        if (this.sortBy === SortOrder.Downloads) {
          apps = sortBy(apps, ['downloads', 'name']).reverse();
        } else {
          apps = apps.sort((a, b) => b.rate * b.ratings - a.rate * a.ratings);
        }
      }

      this.apps = apps;
      this.appListLength.emit(this.apps.length);
    }
  }
  ngOnChanges() {
    this.changeList();
  }
}
