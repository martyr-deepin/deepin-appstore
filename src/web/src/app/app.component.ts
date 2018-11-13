import { Component, OnInit, ViewChild, ElementRef, NgZone } from '@angular/core';
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
  @ViewChild('$context')
  contentRef: ElementRef<HTMLDivElement>;
  updated = false;
  ngOnInit(): void {
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

  switchTheme() {
    this.themeService.getTheme().subscribe(theme => {
      document.body.className = theme;
    });
  }
  // 添加搜索查询索引
  searchIndex() {
    if (BaseService.isNative) {
      this.appService.list().subscribe((apps: App[]) => {
        const appStringList = JSON.stringify(apps);
        Channel.exec('search.updateAppList', appStringList);
      });
    }
  }
  // 等待后台添加索引
  waitUpdate() {
    Channel.connect('storeDaemon.onAppListUpdated').subscribe(resp => {
      this.zone.run(() => {
        this.updated = true;
      });
    });
  }
  searchListen() {
    this.searchService.onOpenApp().subscribe(appName => {
      console.log('open app', appName);
      this.router.navigate(['/app/', appName]);
    });

    this.searchService.onOpenAppList().subscribe(result => {
      console.log('open app list', result.appNameList);
      this.router.navigate(['search'], {
        queryParams: { keyword: result.keyword, apps: result.appNameList },
      });
    });
  }
  screenshotPreview() {
    const img = new Image();
    const canvas = document.createElement('canvas');
    const ctx = canvas.getContext('2d');
    DstoreObject.openOnlineImage().subscribe(src => {
      img.onload = () => {
        canvas.width = img.width;
        canvas.height = img.height;
        ctx.drawImage(img, 0, 0);
        const data = canvas.toDataURL();
        DstoreObject.imageViewer(src, data.slice(data.indexOf(',') + 1));
      };
      img.src = src;
    });
  }
}
