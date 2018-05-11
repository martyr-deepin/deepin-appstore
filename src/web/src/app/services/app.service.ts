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

  private getAppList() {
    return this.getAppMap().pipe(map(appMap => Array.from(appMap.values())));
  }
  private getAppMap() {
    console.log('getList');
    interface AppStat {
      downloadCount: { appName: string; count: number }[];
      rate: { appName: string; rate: number; count: number }[];
    }
    return this.http.get<{ apps: string[] }>(`${this.server}/api/app`).pipe(
      flatMap(resp =>
        forkJoin(
          this.appService.getAppMapByNames(resp.apps),
          this.storeService.getVersion(resp.apps),
          this.http.get<AppStat>(`${this.server}/api/appstat`),
        ),
      ),
      map(([DstoreAppMap, versionList, stat]) => {
        const downloadMap = _.keyBy(stat.downloadCount, 'appName');
        const rateMap = _.keyBy(stat.rate, 'appName');
        const versionMap = _.keyBy(versionList, 'name');
        const appMap = DstoreAppMap as Map<string, App>;
        appMap.forEach(app => {
          app.downloads = _.get(downloadMap, [app.name, 'count'], 0);
          app.rate = _.get(rateMap, [app.name, 'rate']) / 2 || 0;
          app.ratings = _.get(rateMap, [app.name, 'count'], 0);
          app.version = _.get(versionMap, app.name);
          if (!app.version) {
            appMap.delete(app.name);
          }
        });
        return appMap;
      }),
    );
  }

  // 根据分类获取应用列表
  getAppListByCategory(category: string): Observable<App[]> {
    return this.list().pipe(map(apps => apps.filter(app => app.category === category)));
  }
  getApps(appNameList: string[]) {
    return this.appMap().pipe(map(m => appNameList.filter(appName => m.has(appName)).map(m.get)));
  }
  getApp(appName: string) {
    return this.appMap().pipe(map(m => m.get(appName)));
  }
}

export class App extends DstoreApp {
  downloads = 0;
  rate = 0;
  ratings = 0;
  version: AppVersion;
}
