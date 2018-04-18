import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import 'rxjs/add/operator/delay';

import { AppService, App } from '../../services/app.service';
import { StoreService } from '../../services/store.service';
import { BaseService } from '../../dstore/services/base.service';

@Component({
  selector: 'app-update',
  templateUrl: './update.component.html',
  styleUrls: ['./update.component.scss'],
})
export class UpdateComponent implements OnInit {
  server: string;
  upgrade$: Observable<UpgradeResult[]>;
  constructor(
    private storeService: StoreService,
    private appService: AppService,
    private baseService: BaseService,
  ) {}

  ngOnInit() {
    this.server = this.baseService.serverHosts.metadataServer;
    this.upgrade$ = this.storeService.getUpgradableApps().map(list =>
      list.map(appName => ({
        app$: this.appService.getApp(appName + '40'),
        downloadSize$: this.storeService.appDownloadSize(appName),
      })),
    );
  }
}

interface UpgradeResult {
  app$: Observable<App>;
  downloadSize$: Observable<number>;
}
