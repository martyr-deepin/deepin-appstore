import { Component, OnInit, Input, OnChanges, EventEmitter, Output } from '@angular/core';
import { Observable, timer, of } from 'rxjs';
import { map, tap, flatMap } from 'rxjs/operators';
import * as _ from 'lodash';
import { sortBy } from 'lodash';

import { App } from '../../services/app.service';
import { SortOrder } from '../app-title/app-title.component';
import { BaseService } from '../../dstore/services/base.service';
import { StoreService } from '../../services/store.service';
import { StoreJobInfo, AppJobInfo, AppJobStatus } from '../../services/store-job-info';

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
  // 下载任务控制
  start = _.throttle(this.storeService.resumeJob, 1000);
  pause = _.throttle(this.storeService.pauseJob, 1000);
  cancel = _.throttle(this.storeService.clearJob, 1000);

  getAppJob = _.memoize((appName: string): Observable<JobAndStatus> => {
    return timer(0, 1000).pipe(
      flatMap(() => this.storeService.appInstalled(appName)),
      flatMap(
        exists =>
          exists
            ? of({ status: AppJobStatus.finish })
            : this.storeService
                .getJobByName(appName)
                .pipe(
                  map(
                    info =>
                      info
                        ? { status: AppJobStatus.running, job: info }
                        : { status: AppJobStatus.ready },
                  ),
                ),
      ),
    );
  });

  openApp = this.storeService.openApp;
  installApp = (appName: string) => this.storeService.installPackage(appName).subscribe();

  ngOnInit() {}

  ngOnChanges() {
    this.appList$ = this.apps$.pipe(
      map(apps => {
        apps = apps.filter(app => app);
        if (this.sortBy) {
          apps = sortBy(apps, [
            this.sortBy === SortOrder.Downloads ? 'downloads' : 'rate',
            'name',
          ]).reverse();
        }
        if (this.maxCount !== null) {
          apps = apps.slice(0, this.maxCount);
        }
        this.appListLength.emit(apps.length);
        return apps;
      }),
    );
  }
}

interface JobAndStatus {
  status: AppJobStatus;
  jobInfo?: StoreJobInfo;
}
