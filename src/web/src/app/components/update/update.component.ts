import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import 'rxjs/add/operator/delay';
import 'rxjs/add/operator/filter';
import 'rxjs/add/observable/concat';

import { AppService, App } from '../../services/app.service';
import { StoreService } from '../../services/store.service';
import { BaseService } from '../../dstore/services/base.service';

import { StoreJobInfo } from '../../services/store-job-info';

@Component({
  selector: 'app-update',
  templateUrl: './update.component.html',
  styleUrls: ['./update.component.scss'],
})
export class UpdateComponent implements OnInit {
  server: string;
  upgrade$: Observable<any[]>;
  jobs$: Observable<StoreJobInfo>;
  constructor(
    private storeService: StoreService,
    private appService: AppService,
  ) {
    this.server = BaseService.serverHosts.metadataServer;
  }

  ngOnInit() {
    this.upgrade$ = this.storeService.getUpgradableApps();
    // this.upgrade$ = Observable.timer(0, 3000)
    // .mergeMap(() => this.storeService.getUpgradableApps())
    // .map(apps =>
    //   apps.map(appName => {
    //     return {
    //       appName: appName,
    //       app$: this.appService.getApp(appName + '40'),
    //       downloadSize$: this.storeService.appDownloadSize(appName),
    //       job$: this.storeService
    //         .getJobList()
    //         .do(jobs => console.log('jobs', jobs))
    //         .mergeMap(jobs =>
    //           Observable.concat(
    //             ...jobs.map(job => this.storeService.getJobInfo(job)),
    //           ),
    //         )
    //         .do(r => console.log('jobInfos', r, r.name, appName))
    //         .filter(jobInfo => jobInfo.name === appName),
    //     };
    //   }),
    // );
  }

  update(appName: string) {
    this.storeService.installPackage(appName).subscribe(job => {
      console.log('update');
      console.log(job);
    });
  }
}

interface UpgradeResult {
  appName: string;
  app$: Observable<App>;
  downloadSize$: Observable<number>;
  job$: Observable<StoreJobInfo>;
}
