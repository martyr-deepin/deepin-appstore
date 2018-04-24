import { Component, OnInit } from '@angular/core';

import { StoreService } from '../../services/store.service';
import { Observable } from 'rxjs/Observable';
import { StoreJobInfo } from '../../services/store-job-info';

@Component({
  selector: 'app-uninstall',
  templateUrl: './uninstall.component.html',
  styleUrls: ['./uninstall.component.scss'],
})
export class UninstallComponent implements OnInit {
  constructor(private storeService: StoreService) {}

  a$: Observable<any[]>;
  j$: Observable<StoreJobInfo>;
  ngOnInit() {
    this.a$ = this.storeService.getUpgradableApps().map(apps => {
      return apps.map(appName => ({
        appName,
        size$: this.storeService.appDownloadSize(appName),
      }));
    });
  }
}
