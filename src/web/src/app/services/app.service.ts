import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, combineLatest, Subject } from 'rxjs';
import { map, scan, share, debounceTime, switchMap } from 'rxjs/operators';

import { BaseService } from '../dstore/services/base.service';
import { AppService as DstoreAppService } from '../dstore/services/app.service';
import { App as DstoreApp } from '../dstore/services/app';
import { AppVersion } from 'app/modules/client/models/app-version';
import { AppStatService, AppStat } from './stat.service';
import { StoreService } from 'app/modules/client/services/store.service';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class AppService {
  constructor(
    private http: HttpClient,
    private appService: DstoreAppService,
    private appStatService: AppStatService,
    private storeService: StoreService,
  ) {}
  private native = Boolean(window['dstore']);
  private server = BaseService.serverHosts.operationServer;
  activeList = this.http.get<{ apps: string[] }>(`${this.server}/api/app`).toPromise();

  getAppMap() {
    return combineLatest(
      this.appService.getAppList(),
      this.activeList,
      this.appStatService.appStatMap,
    ).pipe(
      map(([dstoreApps, { apps }, statMap]) => {
        const list = dstoreApps
          .filter(app => apps.includes(app.name))
          .map((app: App) => {
            const stat = statMap.get(app.name) || new AppStat();
            app.downloads = stat.downloads;
            app.rate = stat.rate;
            app.ratings = stat.votes;
            return [app.name, app];
          }) as Array<[string, App]>;
        return new Map(list);
      }),
    );
  }

  list(): Observable<App[]> {
    return this.getAppMap().pipe(map(appMap => Array.from(appMap.values())));
  }
  // 根据分类获取应用列表
  getAppListByCategory(category: string): Observable<App[]> {
    return this.list().pipe(map(apps => apps.filter(app => app.category === category)));
  }
  getApps(appNameList: string[]): Observable<App[]> {
    if (!this.native) {
      return this.getAppMap().pipe(
        map(appMap => {
          return appNameList.map(name => appMap.get(name)).filter(Boolean);
        }),
      );
    }
    return combineLatest(this.getAppMap(), this.storeService.getVersionMap(appNameList)).pipe(
      map(([appMap, versionMap]) => {
        const apps = appNameList
          .filter(name => versionMap.has(name) && appMap.has(name))
          .map(name => {
            const app = appMap.get(name);
            app.version = versionMap.get(name);
            return app;
          });
        return apps;
      }),
    );
  }

  getApp(appName: string): Observable<App> {
    return this.getApps([appName]).pipe(map(apps => apps[0]));
  }

  // 附加app信息到一个对象
  addApp<T extends Object>(obj: T, app: App): T & HasApp {
    obj['app'] = app;
    return obj as T & HasApp;
  }
}
export interface HasApp {
  app: App;
}

export class App extends DstoreApp {
  downloads = 0;
  rate = 0;
  ratings = 0;
  version: AppVersion;
}
