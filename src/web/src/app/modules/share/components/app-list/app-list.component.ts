import {
  Component,
  OnInit,
  Input,
  OnChanges,
  EventEmitter,
  Output,
  OnDestroy,
} from '@angular/core';
import { Observable, timer, of, forkJoin, merge, from, Subscription } from 'rxjs';
import { map, tap, flatMap, shareReplay, switchMap, concat, startWith } from 'rxjs/operators';
import * as _ from 'lodash';
import { sortBy } from 'lodash';

import { App } from 'app/services/app.service';
import { SortOrder } from '../app-title/app-title.component';
import { BaseService } from 'app/dstore/services/base.service';
import { StoreService } from 'app/modules/client/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
} from 'app/modules/client/models/store-job-info';
import { AppVersion } from 'app/modules/client/models/app-version';
import { AppService } from 'app/services/app.service';
import { ActivatedRoute, Router } from '@angular/router';
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
  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  // input and output
  @Input()
  apps$: Observable<App[]>;
  @Input()
  appList: App[];
  @Input()
  routePrefix: string;
  @Input()
  sortBy: SortOrder;
  @Input()
  maxCount: number;
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
    if (this.appList) {
      this.apps = this.appList;
    }
    this.jobs$ = this.jobService.jobsInfo().subscribe(jobInfos => {
      const jobs: { [key: string]: StoreJobInfo } = {};
      jobInfos.forEach(job => {
        job.names.forEach(name => {
          jobs[name] = job;
          this.jobNames.add(name);
        });
      });

      this.storeService.getVersion(Object.keys(this.jobs)).subscribe(versions => {
        const vMap = new Map(versions.map(v => [v.name, v] as [string, AppVersion]));
        if (this.apps) {
          this.apps.forEach(app => {
            if (vMap.has(app.name)) {
              app.version = vMap.get(app.name);
            }
          });
        }
        this.jobs = jobs;
      });
    });
  }
  ngOnDestroy() {
    if (this.jobs$) {
      this.jobs$.unsubscribe();
    }
  }

  ngOnChanges() {
    if (this.apps$) {
      this.apps$.subscribe(async apps => {
        apps = apps.filter(Boolean);
        if (this.sortBy) {
          if (this.sortBy === SortOrder.Downloads) {
            apps = sortBy(apps, ['downloads', 'name']).reverse();
          } else {
            apps = apps.sort((a, b) => b.rate * b.ratings - a.rate * a.ratings);
          }
        }
        if (this.maxCount) {
          apps = apps.slice(0, this.maxCount);
        }

        if (BaseService.isNative) {
          const versionMap = await this.storeService
            .getVersionMap(apps.map(app => app.name))
            .toPromise();
          apps = apps
            .filter(app => versionMap.has(app.name))
            .map(app => {
              app.version = versionMap.get(app.name);
              return app;
            });
        }

        this.apps = apps;
        this.appListLength.emit(this.apps.length);
      });
    }
  }

  installApp(app: App) {
    this.installApps.add(app.name);
    this.storeService.installPackage(app.name, app.localInfo.description.name).subscribe();
  }
  updateApp(app: App) {
    this.storeService.updatePackage(app.name, app.localInfo.description.name).subscribe();
  }

  // Show 'open' button only if app open method is 'desktop'.
  appOpenable(app: App): boolean {
    return app.extra.open === 'desktop';
  }
}
