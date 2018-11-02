import { Component, OnInit, ViewChild, ElementRef, Renderer } from '@angular/core';
import { Router } from '@angular/router';

import { BaseService } from './dstore/services/base.service';
import { AppService } from './services/app.service';
import { SearchService } from './services/search.service';
import { Channel } from './dstore-client.module/utils/channel';
import { App } from './dstore/services/app';
import { DstoreObject } from './dstore-client.module/utils/dstore-objects';
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
  ) {}
  @ViewChild('$context')
  contentRef: ElementRef<HTMLDivElement>;

  ngOnInit(): void {
    this.searchIndex();
    this.searchListen();
    this.screenshotPreview();
    this.switchTheme();
  }

  switchTheme() {
    this.themeService.getTheme().subscribe(theme => {
      document.body.className = theme;
    });
  }

  searchIndex() {
    if (BaseService.isNative) {
      this.appService.list().subscribe((apps: App[]) => {
        const appStringList = JSON.stringify(apps);
        Channel.exec('search.updateAppList', appStringList);
      });
    }
  }
  searchListen() {
    this.searchService.onOpenApp().subscribe(appName => {
      console.log('open app', appName);
      this.router.navigate(['search', appName]);
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
