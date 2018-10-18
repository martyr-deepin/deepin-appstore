import { Injectable, OnInit } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';

import { Observable, Subject, forkJoin, from, of } from 'rxjs';
import {
  retry,
  shareReplay,
  map,
  tap,
  flatMap,
  scan,
  switchMap,
  last,
  catchError,
} from 'rxjs/operators';

import { throttle, filter, compact, chain, get, cloneDeep, defaults } from 'lodash';

import * as localForage from 'localforage';

import { BaseService } from './base.service';
import { CategoryService } from './category.service';

import { App, appReviver } from './app';
import { Error, ErrorCode } from './errno';
import { Locale } from '../utils/locale';

@Injectable()
export class AppService {
  private readonly appsStoreKey = 'apps-apps';
  private readonly paramsStoreKey = 'apps-params  ';
  private readonly metadataService = BaseService.serverHosts.metadataServer;
  private readonly isNative = BaseService.isNative;
  private readonly apiURL = `${this.metadataService}/api/app`;
  private readonly store = localForage.createInstance({ name: 'apps' });
  private appMap$ = from(this.getAppMap());

  constructor(private http: HttpClient, private categoryServer: CategoryService) {}

  private async getAppMap() {
    const result = await this.http
      .get(this.apiURL, { responseType: 'text' })
      .pipe(
        retry(3),
        map(body => JSON.parse(body, appReviver) as Result),
      )
      .toPromise();

    if (!result || result.error) {
      throw result;
    }
    const appMap = result.apps.reduce(
      (acc, app) => Object.assign(acc, { [app.name]: app }),
      {} as AppMap,
    );
    const categoryList = await this.categoryServer.getList().toPromise();
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
    return appMap;
  }

  // 获取全部应用列表
  getAppList(): Observable<App[]> {
    return this.appMap$.pipe(map(m => compact(Object.values(m))));
  }

  // 根据应用名获取应用
  getAppByName(name: string): Observable<App> {
    return this.appMap$.pipe(
      map(m => m[name]),
      tap(app => console.log(`getAppByName:(${name}):`, app)),
    );
  }

  // 根据应用名列表获取应用列表
  getAppListByNames(appNames: string[]): Observable<App[]> {
    return this.appMap$.pipe(map(m => appNames.map(appName => m[appName])));
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
