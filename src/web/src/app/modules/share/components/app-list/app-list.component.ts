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
export class AppListComponent implements OnInit, OnChanges, OnDestroy {
  constructor(private storeService: StoreService, private jobService: JobService) {}
  // const
  server = BaseService.serverHosts.metadataServer;

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
  jobs: { [key: string]: StoreJobInfo } = {};
  jobNames = new Set<string>();
  installApps = new Set<string>();
  jobs$: Subscription;
  loading = false;

  // job control
  start = this.storeService.resumeJob;
  pause = this.storeService.pauseJob;
  openApp = this.storeService.openApp;

  ngOnInit() {
    this.jobs$ = this.jobService.jobsInfo().subscribe(jobInfos => {
      const jobs: { [key: string]: StoreJobInfo } = {};
      console.log(jobInfos);
      jobInfos.forEach(job => {
        job.names.forEach(name => {
          jobs[name] = job;
          this.jobNames.add(name);
        });
      });
    });
    this.changeList();
  }
  ngOnDestroy() {
    if (this.jobs$) {
      this.jobs$.unsubscribe();
    }
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
