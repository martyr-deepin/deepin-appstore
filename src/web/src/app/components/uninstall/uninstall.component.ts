import { Component, OnInit } from '@angular/core';

import { StoreService } from '../../services/store.service';
import { Observable } from 'rxjs/Observable';
import { map } from 'rxjs/operators';

import { StoreJobInfo } from '../../services/store-job-info';
import { AppService, App } from '../../services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { timer, forkJoin } from 'rxjs';
import { flatMap } from 'rxjs/operators';

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
