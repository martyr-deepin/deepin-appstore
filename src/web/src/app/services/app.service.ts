import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import * as localForage from 'localforage';
import * as _ from 'lodash';
import { Observable, forkJoin, of, from } from 'rxjs';
import { map, flatMap, tap, share, shareReplay, retry, filter, catchError } from 'rxjs/operators';

import { BaseService } from '../dstore/services/base.service';
import { AppService as DstoreAppService } from '../dstore/services/app.service';
import { App as DstoreApp } from '../dstore/services/app';
import { StoreService } from '../dstore-client.module/services/store.service';
import { AppVersion } from '../dstore-client.module/models/app-version';
import { version } from 'punycode';

@Injectable()
export class AppService {
  constructor(
    private http: HttpClient,
    private appService: DstoreAppService,
    private storeService: StoreService,
  ) {}
  private server = BaseService.serverHosts.operationServer;
  private store = localForage.createInstance({ name: 'client-apps' });

  list = _.throttle(this.getAppList, 1000 * 30);
  appMap = _.throttle(this.getAppMap, 1000 * 30);
  listNoVersion(): Observable<App[]> {
    return this.getAppMapNoVersion().pipe(map(appMap => Array.from(appMap.values())));
  }
  private getAppList(): Observable<App[]> {
    return this.getAppMap().pipe(map(appMap => Array.from(appMap.values())));
  }
  private getAppMap(): Observable<Map<string, App>> {
    if (!BaseService.isNative) {
      return this.getAppMapNoVersion().pipe(shareReplay());
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
      shareReplay(),
    );
  }
  private getAppMapNoVersion() {
    interface AppStat {
      downloadCount: { appName: string; count: number }[];
      rate: { appName: string; rate: number; count: number }[];
    }
    return forkJoin(
      this.appService.getAppList(),
      this.http.get<{ apps: string[] }>(`${this.server}/api/app`),
      this.http.get<AppStat>(`${this.server}/api/appstat`),
    ).pipe(
      map(([dstoreApps, { apps }, stat]) => {
        const downloadMap = _.keyBy(stat.downloadCount, 'appName');
        const rateMap = _.keyBy(stat.rate, 'appName');
        const list = dstoreApps.filter(app => apps.includes(app.name)).map((app: App) => {
          app.downloads = _.get(downloadMap, [app.name, 'count'], 0);
          app.rate = _.get(rateMap, [app.name, 'rate']) / 2 || 0;
          app.ratings = _.get(rateMap, [app.name, 'count'], 0);
          return [app.name, app];
        }) as Array<[string, App]>;
        this.store.setItem('apps', list);
        return new Map(list);
      }),
      catchError(() => {
        return from(this.store.getItem<Array<[string, App]>>('apps')).pipe(
          map(value => new Map(value)),
        );
      }),
    );
  }

  // 根据分类获取应用列表
  getAppListByCategory(category: string): Observable<App[]> {
    return this.list().pipe(
      tap(apps => console.log(apps)),
      map(apps => apps.filter(app => app.category === category)),
    );
  }
  getApps(appNameList: string[]): Observable<App[]> {
    return this.appMap().pipe(map(m => appNameList.filter(m.has.bind(m)).map(m.get.bind(m))));
  }
  getApp(appName: string): Observable<App> {
    return this.appMap().pipe(
      map(m => m.get(appName)),
      tap(app => console.log('getApp', app)),
    );
  }
}

export class App extends DstoreApp {
  downloads = 0;
  rate = 0;
  ratings = 0;
  version: AppVersion;
}
