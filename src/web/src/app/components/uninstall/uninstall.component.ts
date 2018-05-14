import { Component, OnInit } from '@angular/core';

import { StoreService } from '../../services/store.service';
import { Observable } from 'rxjs/Observable';
import { map } from 'rxjs/operators';

import { StoreJobInfo } from '../../services/store-job-info';
import { AppService, App } from '../../services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { timer } from 'rxjs';
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
  ngOnInit() {
    this.uninstallApps$ = timer(0, 1000).pipe(
      flatMap(() =>
        this.appService.list().pipe(map(apps => apps.filter(app => app.version.localVersion))),
      ),
    );
  }
  uninstall(appName: string) {
    this.storeService
      .removePackage(appName)
      .subscribe(() => console.log('卸载成功'), () => console.log('卸载失败'));
  }
}
