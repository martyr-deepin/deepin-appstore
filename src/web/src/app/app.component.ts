import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { Router, NavigationEnd } from '@angular/router';
import { Observable } from 'rxjs/Observable';

import { BaseService } from './dstore/services/base.service';
import { StoreService } from './dstore-client.module/services/store.service';
import { AppService } from './services/app.service';
import { SearchService, SearchResult } from './services/search.service';
import { Channel } from './dstore-client.module/utils/channel';
import { App } from './dstore/services/app';
import { filter } from 'rxjs/operators';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent implements OnInit {
  constructor(
    private router: Router,
    private appService: AppService,
    private searchService: SearchService,
  ) {}
  @ViewChild('$context') contentRef: ElementRef<HTMLDivElement>;

  ngOnInit(): void {
    this.router.events.pipe(filter(event => event instanceof NavigationEnd)).subscribe(() => {
      console.log('router');
      this.contentRef.nativeElement.scrollTop = 0;
    });

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
