import { debounceTime, filter } from 'rxjs/operators';
import { Component, OnInit, NgZone } from '@angular/core';
import { Router } from '@angular/router';

import { BaseService } from './dstore/services/base.service';
import { AppService } from './services/app.service';
import { SearchService } from './services/search.service';
import { Channel } from 'app/modules/client/utils/channel';
import { App } from './dstore/services/app';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { ThemeService } from './services/theme.service';

@Component({
  selector: 'dstore-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent implements OnInit {
  constructor(
    private router: Router,
    private appService: AppService,
    private searchService: SearchService,
    private themeService: ThemeService,
    private zone: NgZone,
  ) {}
  updated = false;
  ngOnInit(): void {
    this.connectToRouter('menu.appsRequested', '/my/apps');
    this.connectToRouter('menu.commentRequested', '/my/comments');
    this.connectToRouter('menu.rewardRequested', '/my/donates');
    this.searchIndex();
    this.searchListen();
    this.screenshotPreview();
    this.switchTheme();
    if (!BaseService.isNative) {
      this.updated = true;
    } else {
      this.waitUpdate();
    }
  }

  connectToRouter(signal: string, url: string) {
    Channel.connect(signal).subscribe(() => {
      this.zone.run(() => {
        this.router.navigateByUrl(url);
      });
    });
  }

  switchTheme() {
    this.themeService.getTheme().subscribe(theme => {
      document.body.className = theme;
    });
  }
  // 添加搜索查询索引
  searchIndex() {
    if (BaseService.isNative) {
      this.appService
        .list()
        .pipe(
          debounceTime(1000),
          filter(Boolean),
        )
        .subscribe((apps: App[]) => {
          const appStringList = JSON.stringify(apps);
          Channel.exec('search.updateAppList', appStringList);
        });
    }
  }
  // 等待后台添加索引
  waitUpdate() {
    Channel.connect('search.onAppListUpdated').subscribe(resp => {
      this.zone.run(() => {
        this.updated = true;
      });
    });
  }
  searchListen() {
    this.searchService.onOpenApp().subscribe(appName => {
      this.router.navigate(['/app/', appName]);
    });

    this.searchService.onOpenAppList().subscribe(result => {
      this.router.navigate(['search'], {
        queryParams: { keyword: result.keyword, apps: result.appNameList },
      });
    });
  }
  screenshotPreview() {
    DstoreObject.openOnlineImage().subscribe(src => {
      fetch(src)
        .then(resp => resp.blob())
        .then(blob => {
          return new Promise<string>(resolve => {
            const reader = new FileReader();
            reader.onload = () => resolve(reader.result as string);
            reader.readAsDataURL(blob);
          });
        })
        .then(data => {
          DstoreObject.imageViewer(src, data.slice(data.indexOf(',') + 1));
        });
    });
  }
}
