import { debounceTime, filter, retry } from 'rxjs/operators';
import { Component, OnInit, NgZone } from '@angular/core';
import { Router } from '@angular/router';

import { Channel } from 'app/modules/client/utils/channel';
import { BaseService } from 'app/dstore/services/base.service';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';

import { ThemeService } from 'app/services/theme.service';
import { App, AppService } from 'app/services/app.service';
import { SearchService } from 'app/services/search.service';
import { SysFontService } from 'app/services/sys-font.service';
import { MenuService } from './services/menu.service';

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
    private sysFontService: SysFontService,
    private menu: MenuService,
  ) {}
  ngOnInit(): void {
    this.switchTheme();
    if (!BaseService.isNative) {
      return;
    }
    this.searchIndex();
    this.searchListen();
    this.screenshotPreview();
    this.switchFont();
    this.menu.serve();
  }

  switchTheme() {
    console.log('test');
    this.themeService.getTheme().subscribe(console.log);
    this.themeService.getTheme().subscribe(theme => {
      console.log('getTheme', theme);
      document.body.className = theme;
    });
  }
  switchFont() {
    this.sysFontService.fontChange$.subscribe(([fontFamily, fontSize]) => {
      const HTMLGlobal = document.querySelector('html');
      HTMLGlobal.style.fontFamily = fontFamily;
      HTMLGlobal.style.fontSize = fontSize + 'px';
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
  // 搜索结果显示
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
