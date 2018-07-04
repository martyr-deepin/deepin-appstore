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

import { App } from '../../services/app.service';
import { SortOrder } from '../app-title/app-title.component';
import { BaseService } from '../../dstore/services/base.service';
import { StoreService } from '../../dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
} from '../../dstore-client.module/models/store-job-info';
import { AppVersion } from '../../dstore-client.module/models/app-version';
import { AppService } from '../../services/app.service';
import { OffsetService } from '../../services/offset.service';
import { ActivatedRoute, Router } from '@angular/router';

@Component({
  selector: 'app-app-list',
  templateUrl: './app-list.component.html',
  styleUrls: ['./app-list.component.scss'],
})
export class AppListComponent implements OnInit, OnChanges, OnDestroy {
  constructor(
    private appService: AppService,
    private storeService: StoreService,
    private offsetService: OffsetService,
    private router: Router,
  ) {}
  // const
  server = BaseService.serverHosts.metadataServer;
  StoreJobStatus = StoreJobStatus;
  StoreJobType = StoreJobType;

  // input and output
  @Input() apps$: Observable<App[]>;
  @Input() sortBy: SortOrder;
  @Input() maxCount: number;
  @Input() rankIndex: boolean;
  @Input() subtitle = 'category';
  @Output() appListLength = new EventEmitter<number>(true);

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
    this.jobs$ = merge(this.storeService.getJobList(), this.storeService.jobListChange())
      .pipe(
        switchMap(jobs => {
          if (jobs.length > 0) {
            return timer(0, 1000).pipe(flatMap(() => this.storeService.getJobsInfo(jobs)));
          } else {
            return of([] as StoreJobInfo[]);
          }
        }),
      )
      .subscribe(jobInfos => {
        const jobs: { [key: string]: StoreJobInfo } = {};
        jobInfos.forEach(job => {
          job.names.forEach(name => {
            jobs[name] = job;
            this.jobNames.add(name);
          });
        });

        this.jobs = jobs;
        this.storeService.getVersion(Object.keys(this.jobs)).subscribe(versions => {
          const vMap = new Map(versions.map(v => [v.name, v] as [string, AppVersion]));
          if (this.apps) {
            this.apps.forEach(app => {
              if (vMap.has(app.name)) {
                app.version = vMap.get(app.name);
              }
            });
          }
        });
      });
  }
  ngOnDestroy() {
    this.jobs$.unsubscribe();
  }

  ngOnChanges() {
    if (this.apps$) {
      this.loading = true;
      this.apps$.subscribe(apps => {
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
        this.apps = apps;
        this.appListLength.emit(this.apps.length);
        this.loading = false;
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
