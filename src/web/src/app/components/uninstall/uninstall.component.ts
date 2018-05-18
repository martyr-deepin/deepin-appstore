import { Component, OnInit } from '@angular/core';

import { Observable, timer, forkJoin } from 'rxjs';
import { map, flatMap, switchMap } from 'rxjs/operators';

import * as _ from 'lodash';

import { StoreService } from '../../services/store.service';
import { StoreJobInfo } from '../../services/store-job-info';
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
    );
    this.uninstallJobMap$ = timer(0, 1000).pipe(
      flatMap(() => this.storeService.getJobListInfo()),
      map(jobs => {
        return new Map(
          jobs
            .filter(job => job.type === 'remove')
            .map(job => [job.name, job] as [string, StoreJobInfo]),
        );
      }),
    );
  }
  uninstall(appName: string) {
    this.storeService.removePackage(appName).subscribe();
  }
}
