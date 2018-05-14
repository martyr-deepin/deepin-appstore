import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { Observable } from 'rxjs/Observable';

import { BaseService } from './dstore/services/base.service';
import { StoreService } from './services/store.service';
import { AppService } from './services/app.service';
import { SearchService, SearchResult } from './services/search.service';
import { Channel } from './utils/channel';
import { App } from './dstore/services/app';

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
    private searchService: SearchService,
  ) {}

  ngOnInit(): void {
    if (BaseService.isNative) {
      this.appService.listNoVersion().subscribe((apps: App[]) => {
        const appStringList = JSON.stringify(apps);
        Channel.exec('search.updateAppList', appStringList);
      });
    }

    this.searchService.onOpenApp().subscribe(appName => {
      console.log('open app', appName);
      this.router.navigate(['search', appName]);
    });

    this.searchService.onOpenAppList().subscribe(result => {
      console.log('open app list', result.appNameList);
      this.router.navigate(['search', { keyword: result.keyword, apps: result.appNameList }]);
    });
  }
}
