import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import * as _ from 'lodash';
import { Observable, forkJoin, of } from 'rxjs';
import { map, flatMap, tap, share, shareReplay, retry, filter } from 'rxjs/operators';

import { BaseService } from '../dstore/services/base.service';
import { AppService as DstoreAppService } from '../dstore/services/app.service';
import { App as DstoreApp } from '../dstore/services/app';
import { StoreService } from './store.service';
import { AppVersion } from './app-version';
import { version } from 'punycode';

@Injectable()
export class AppService {
  constructor(
    private http: HttpClient,
    private appService: DstoreAppService,
    private storeService: StoreService,
  ) {}

  private server = BaseService.serverHosts.operationServer;

  list = _.throttle(this.getAppList, 1000 * 10);
  appMap = _.throttle(this.getAppMap, 1000 * 10);
  listNoVersion(): Observable<App[]> {
    return this.getAppMapNoVersion().pipe(map(appMap => Array.from(appMap.values())));
  }
  private getAppList(): Observable<App[]> {
    return this.getAppMap().pipe(map(appMap => Array.from(appMap.values())));
  }
  private getAppMap(): Observable<Map<string, App>> {
    if (!BaseService.isNative) {
      return this.getAppMapNoVersion();
    }
    return this.getAppMapNoVersion().pipe(
      flatMap(appMap => {
        return this.storeService.getVersion(Array.from(appMap.keys())).pipe(
          map(versionList => {
            const versionMap = _.keyBy(versionList, 'name');
            appMap.forEach(app => {
              if (versionMap[app.name]) {
                app.version = versionMap[app.name];
              } else {
                appMap.delete(app.name);
              }
            });
            return appMap;
          }),
        );
      }),
    );
  }
  private getAppMapNoVersion(): Observable<Map<string, App>> {
    console.log('getList');
    interface AppStat {
      downloadCount: { appName: string; count: number }[];
      rate: { appName: string; rate: number; count: number }[];
    }
    return this.http.get<{ apps: string[] }>(`${this.server}/api/app`).pipe(
      flatMap(resp =>
        forkJoin(
          this.appService.getAppMapByNames(resp.apps),
          this.http.get<AppStat>(`${this.server}/api/appstat`),
        ),
      ),
      map(([DstoreAppMap, stat]) => {
        const downloadMap = _.keyBy(stat.downloadCount, 'appName');
        const rateMap = _.keyBy(stat.rate, 'appName');
        const appMap = DstoreAppMap as Map<string, App>;
        appMap.forEach(app => {
          app.downloads = _.get(downloadMap, [app.name, 'count'], 0);
          app.rate = _.get(rateMap, [app.name, 'rate']) / 2 || 0;
          app.ratings = _.get(rateMap, [app.name, 'count'], 0);
        });
        return appMap;
      }),
    );
  }

  // 根据分类获取应用列表
  getAppListByCategory(category: string): Observable<App[]> {
    return this.list().pipe(map(apps => apps.filter(app => app.category === category)));
  }
  getApps(appNameList: string[]): Observable<App[]> {
    return this.appMap().pipe(map(m => appNameList.filter(m.has.bind(m)).map(m.get.bind(m))));
  }
  getApp(appName: string): Observable<App> {
    return this.appMap().pipe(map(m => m.get(appName)), tap(app => console.log('getApp', app)));
  }
}

export class App extends DstoreApp {
  downloads = 0;
  rate = 0;
  ratings = 0;
  version: AppVersion;
}
