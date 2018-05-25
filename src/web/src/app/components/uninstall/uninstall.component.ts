import { Component, OnInit } from '@angular/core';

import { Observable, timer, forkJoin } from 'rxjs';
import { map, flatMap, switchMap } from 'rxjs/operators';

import * as _ from 'lodash';

import { StoreService } from '../../dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  StoreJobStatus,
  StoreJobType,
  AppJobStatus,
} from '../../dstore-client.module/models/store-job-info';
import { AppService, App } from '../../services/app.service';
import { BaseService } from '../../dstore/services/base.service';

@Component({
  selector: 'app-uninstall',
  templateUrl: './uninstall.component.html',
  styleUrls: ['./uninstall.component.scss'],
})
export class UninstallComponent implements OnInit {
  constructor(private storeService: StoreService, private appService: AppService) {}
  metadataServer = BaseService.serverHosts.metadataServer;

  // uninstallApps$: Observable<App[]>;
  // uninstallJobMap$: Observable<Map<string, StoreJobInfo>>;
  // getInstalledTime = _.memoize(appName => this.storeService.getInstalledTime(appName));

  uninstallApps$: Observable<UninstallApp[]>;
  ngOnInit() {
    this.uninstallApps$ = timer(0, 1000).pipe(
      switchMap(() => this.appService.list()),
      flatMap(
        apps => this.storeService.getVersionMap(apps.map(app => app.name)),
        (apps, vMap) => {
          return apps
            .map(app => {
              app.version = vMap.get(app.name);
              return app;
            })
            .filter(app => app.version && app.version.localVersion);
        },
      ),
      flatMap(
        apps =>
          forkJoin(
            this.storeService.getJobInfoMap(),
            this.storeService.getInstalledTimeMap(apps.map(app => app.name)),
          ),
        (apps: UninstallApp[], [jobMap, timeMap]) => {
          console.log(jobMap);
          apps.forEach(app => {
            const job = jobMap.get(app.name);
            if (
              job &&
              job.type === StoreJobType.uninstall &&
              job.status !== StoreJobStatus.failed
            ) {
              app.jobInfo = job;
            } else {
              app.jobInfo = null;
            }
            app.installedTime = timeMap.get(app.name);
            return app;
          });
          return _.sortBy(apps, 'installedTime').reverse();
        },
      ),
    );
  }

  uninstall(appName: string) {
    this.storeService.removePackage(appName).subscribe();
  }
}

interface UninstallApp extends App {
  installedTime: number;
  jobInfo: StoreJobInfo;
}
