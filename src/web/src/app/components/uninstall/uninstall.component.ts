import { Component, OnInit } from '@angular/core';

import { Observable, timer, forkJoin } from 'rxjs';
import { map, flatMap, switchMap } from 'rxjs/operators';

import * as _ from 'lodash';

import { StoreService } from '../../dstore-client.module/services/store.service';
import {
  StoreJobInfo,
  StoreJobStatus,
  StoreJobType,
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
  uninstallApps$: Observable<App[]>;
  uninstallJobMap$: Observable<Map<string, StoreJobInfo>>;
  getInstalledTime = _.memoize(appName => this.storeService.getInstalledTime(appName));

  ngOnInit() {
    this.uninstallApps$ = timer(0, 1000).pipe(
      flatMap(() => this.appService.list()),
      map(apps => apps.filter(app => app.version.localVersion)),
      flatMap(apps =>
        this.storeService.getVersion(apps.map(app => app.name)).pipe(
          map(versions => {
            const versionMap = _.keyBy(versions, 'name');
            return apps.filter(app => versionMap[app.name] && versionMap[app.name].localVersion);
          }),
        ),
      ),
    );
    this.uninstallJobMap$ = timer(0, 1000).pipe(
      flatMap(() => this.storeService.getJobListInfo()),
      map(jobs => {
        return new Map(
          jobs
            .filter(
              job => job.type === StoreJobType.uninstall && job.status !== StoreJobStatus.failed,
            )
            .map(job => [job.name, job] as [string, StoreJobInfo]),
        );
      }),
    );
  }
  uninstall(appName: string) {
    this.storeService.removePackage(appName).subscribe();
  }
}
