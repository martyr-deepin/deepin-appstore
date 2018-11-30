import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';

import { Observable, from, Subject } from 'rxjs';
import { retry, map, tap, last, shareReplay } from 'rxjs/operators';

import { compact, get, cloneDeep } from 'lodash';

import * as localForage from 'localforage';

import { CategoryService } from './category.service';

import { App, appReviver } from './app';
import { Error } from './errno';
import { Locale } from '../utils/locale';

const DSTORE_VERSION = '5.1.2.2';

@Injectable({
  providedIn: 'root',
})
export class AppService {
  private readonly apiURL = `${environment.metadataServer}/api/app`;
  private readonly appMap$ = new Subject<AppMap>();
  private readonly store = localForage.createInstance({ name: 'apps' });
  categoryList$ = this.categoryServer.getList().toPromise();
  constructor(private http: HttpClient, private categoryServer: CategoryService) {
    this.syncAppMap();
  }
  private getApps(url: string, old: App[] = null, since: string = '') {
    return this.http
      .get(url, { responseType: 'text' })
      .toPromise()
      .then(body => JSON.parse(body, appReviver) as Result)
      .then(result => {
        if (old && old.length > 0) {
          if (since === result.lastModified) {
            return old;
          }
          localStorage.setItem('since', result.lastModified);
          if (result.deleted && result.deleted.length > 0) {
            old = old.filter(app => !result.deleted.includes(app.id));
          }
          if (result.apps && result.apps.length > 0) {
            old = old.concat(result.apps);
          }
          return old;
        }
        return result.apps;
      });
  }
  private async setApps(apps: App[]) {
    apps = cloneDeep(apps);
    const categoryList = await this.categoryList$;
    const appMap = apps.reduce((acc, app) => Object.assign(acc, { [app.name]: app }), {} as AppMap);
    // 添加快捷访问
    Object.values(appMap).forEach(app => {
      app.icon = environment.metadataServer + '/' + app.icon;
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
    console.log('sync app list');
    if (localStorage.getItem('DSTORE_VERSION') !== DSTORE_VERSION) {
      localStorage.setItem('DSTORE_VERSION', DSTORE_VERSION);
      await this.store.clear();
    }
    try {
      // 从缓存获取应用列表
      let apps = await this.store.getItem<App[]>('apps');
      if (!apps) {
        // 从预存文件获取应用列表
        apps = await this.getApps('/assets/app.json');
      }
      this.setApps(apps);
      // 增量更新应用列表
      const since = localStorage.getItem('since');
      if (since) {
        apps = await this.getApps(this.apiURL + '?since=' + since, apps, since);
        this.setApps(apps);
      }
    } catch (err) {
      console.error(err);
    }

    try {
      // 启动10秒后全量更新apps
      await new Promise(resolve => setTimeout(resolve, 10 * 1000));

      const apps = await this.getApps(this.apiURL);
      if (apps) {
        apps.sort((a, b) => a.name.localeCompare(b.name));
        await this.store.setItem('apps', apps);
        await this.setApps(apps);
      }
    } catch (err) {
      console.error(err);
    }
  }

  getAppMap() {
    return this.appMap$.pipe(shareReplay(1));
  }
  // 获取全部应用列表
  getAppList(): Observable<App[]> {
    return this.getAppMap().pipe(map(m => Object.values(m).filter(Boolean)));
  }

  // 根据应用名获取应用
  getAppByName(name: string): Observable<App> {
    return this.getAppMap().pipe(map(m => m[name]));
  }

  // 根据应用名列表获取应用列表
  getAppListByNames(appNames: string[]): Observable<App[]> {
    return this.getAppMap().pipe(map(m => appNames.map(appName => m[appName]).filter(Boolean)));
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
