import { debounceTime, filter, retry } from 'rxjs/operators';
import { Component, OnInit, NgZone, ViewChild, ElementRef } from '@angular/core';
import { Router } from '@angular/router';

import { Channel } from 'app/modules/client/utils/channel';
import { BaseService } from 'app/dstore/services/base.service';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';

import { ThemeService } from 'app/services/theme.service';
import { App, AppService } from 'app/services/app.service';
import { SearchService } from 'app/services/search.service';
import { SysFontService } from 'app/services/sys-font.service';
import { MenuService } from './services/menu.service';
import { KeyboardService } from './services/keyboard.service';

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
    private keyboard: KeyboardService,
  ) {}
  @ViewChild('scrollbarRef') scrollbarRef: ElementRef<HTMLDivElement>;
  ngOnInit(): void {
    if (!BaseService.isNative) {
      return;
    }
    this.switchTheme();
    this.screenshotPreview();
    this.switchFont();
    this.menu.serve();
    this.keyboard.server();
  }
  // switch theme dark or light
  switchTheme() {
    this.themeService.getTheme().subscribe(theme => {
      document.body.className = theme;
    });
  }
  // switch font family and font size
  switchFont() {
    this.sysFontService.fontChange$.subscribe(([fontFamily, fontSize]) => {
      const HTMLGlobal = document.querySelector('html');
      HTMLGlobal.style.fontFamily = fontFamily;
      HTMLGlobal.style.fontSize = fontSize + 'px';
    });
  }
  // preview software screenshot
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
