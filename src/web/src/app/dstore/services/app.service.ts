import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';

import { Observable, from, BehaviorSubject } from 'rxjs';
import { retry, map, tap, filter } from 'rxjs/operators';

import { compact, get } from 'lodash';

import * as localForage from 'localforage';

import { CategoryService } from './category.service';

import { App, appReviver } from './app';
import { Error } from './errno';
import { Locale } from '../utils/locale';

@Injectable()
export class AppService {
  private readonly apiURL = `${environment.metadataServer}/api/app`;
  private readonly appMap$ = new BehaviorSubject<AppMap>(null);
  private readonly store = localForage.createInstance({ name: 'apps' });
  categoryList$ = this.categoryServer.getList().toPromise();
  constructor(private http: HttpClient, private categoryServer: CategoryService) {
    this.syncAppMap();
  }
  private getApps(url: string) {
    return this.http
      .get(url, { responseType: 'text' })
      .pipe(
        map(body => JSON.parse(body, appReviver) as Result),
        map(result => result.apps),
      )
      .toPromise();
  }
  private async setApps(apps: App[]) {
    const categoryList = await this.categoryList$;
    const appMap = apps.reduce((acc, app) => Object.assign(acc, { [app.name]: app }), {} as AppMap);
    // 添加快捷访问
    Object.values(appMap).forEach(app => {
      app.localCategory = categoryList[app.category].LocalName;
      if (get(app, ['locale', Locale.getUnixLocale(), 'description', 'name'])) {
        app.localInfo = app.locale[Locale.getUnixLocale()];
      } else if (get(app, 'locale.en_US.description.name')) {
        app.localInfo = app.locale['en_US'];
      } else {
        app.localInfo = Object.values(app.locale).find(l => Boolean(l.description.name));
      }
      // 填充图片
      if (!app.localInfo.images.cover) {
        const l = Object.values(app.locale).find(
          locale => Boolean(locale.images) && Boolean(locale.images.cover),
        );
        if (l) {
          app.localInfo.images.cover = l.images.cover;
        }
      }
      if (!app.localInfo.images.coverHD) {
        const l = Object.values(app.locale).find(
          locale => Boolean(locale.images) && Boolean(locale.images.coverHD),
        );
        if (l) {
          app.localInfo.images.cover = l.images.cover;
        }
      }
      if (!app.localInfo.images.screenshot || app.localInfo.images.screenshot.length === 0) {
        const l = Object.values(app.locale).find(
          locale =>
            Boolean(locale.images) &&
            Boolean(locale.images.screenshot) &&
            locale.images.screenshot.length > 0,
        );
        if (l) {
          app.localInfo.images.screenshot = l.images.screenshot;
        }
      }
      if (!app.localInfo.images.screenshotHD || app.localInfo.images.screenshotHD.length === 0) {
        const l = Object.values(app.locale).find(
          locale =>
            Boolean(locale.images) &&
            Boolean(locale.images.screenshotHD) &&
            locale.images.screenshotHD.length > 0,
        );
        if (l) {
          app.localInfo.images.screenshotHD = l.images.screenshotHD;
        }
      }
    });
    this.appMap$.next(appMap);
  }
  private async syncAppMap() {
    let apps = await this.store.getItem<App[]>('apps');
    if (!apps) {
      apps = await this.getApps('/assets/app.json');
    }
    await this.setApps(apps);

    apps = await this.getApps(this.apiURL);
    if (apps) {
      apps.sort((a, b) => a.name.localeCompare(b.name));
      this.store.setItem('apps', apps);
      await this.setApps(apps);
    }
  }

  // 获取全部应用列表
  getAppList(): Observable<App[]> {
    return this.appMap$.pipe(
      filter(Boolean),
      map(m => compact(Object.values(m))),
    );
  }

  // 根据应用名获取应用
  getAppByName(name: string): Observable<App> {
    return this.appMap$.pipe(map(m => m[name]));
  }

  // 根据应用名列表获取应用列表
  getAppListByNames(appNames: string[]): Observable<App[]> {
    return this.appMap$.pipe(map(m => appNames.map(appName => m[appName]).filter(Boolean)));
  }
}

interface AppMap {
  [key: string]: App;
}

interface Result {
  lastModified: string;
  apps: App[];
  deleted: number[];
  error: Error;
}
