import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { Observable } from 'rxjs/Observable';

import { BaseService } from './dstore/services/base.service';
import { StoreService } from './services/store.service';
import { AppService } from './services/app.service';
import { App } from './dstore/services/app';
import { Channel } from './utils/channel';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent implements OnInit {
  title = 'app';

  constructor(
    private router: Router,
    private appService: AppService,
    private storeService: StoreService,
  ) {}

  ngOnInit(): void {
    if (BaseService.isNative) {
      this.appService.list().subscribe((apps: App[]) => {
        const appStringList = JSON.stringify(apps);
        Channel.exec('search.updateAppList', appStringList);
      });
    }

    this.storeService.onOpenApp().subscribe(appName => {
      console.log('open app', appName);
      this.router.navigate(['search', appName]);
    });

    this.storeService.onOpenAppList().subscribe(appNameList => {
      console.log('appNameList', appNameList);
      this.router.navigate(['search', { apps: ['1', '2'] }]);
    });
  }
}
