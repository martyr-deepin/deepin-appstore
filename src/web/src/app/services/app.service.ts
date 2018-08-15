import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import * as localForage from 'localforage';
import * as _ from 'lodash';
import { Observable, forkJoin, of, from, zip } from 'rxjs';
import { map, flatMap, tap, share, shareReplay, retry, filter, catchError } from 'rxjs/operators';

import { BaseService } from '../dstore/services/base.service';
import { AppService as DstoreAppService } from '../dstore/services/app.service';
import { App as DstoreApp } from '../dstore/services/app';
import { StoreService } from '../dstore-client.module/services/store.service';
import { AppVersion } from '../dstore-client.module/models/app-version';
import { version } from 'punycode';
import { AppStatService, AppStat } from './stat.service';

@Injectable()
export class AppService {
  constructor(
    private http: HttpClient,
    private appService: DstoreAppService,
    private storeService: StoreService,
    private appStatService: AppStatService,
  ) {
    console.log(BaseService.domainName);
  }
  private server = BaseService.serverHosts.operationServer;
  private store = localForage.createInstance({ name: 'client-apps:' + BaseService.domainName });

  appMap = _.throttle(this.getAppMap, 1000 * 30);
  listNoVersion(): Observable<App[]> {
    return this.getAppMapNoVersion().pipe(map(appMap => Array.from(appMap.values())));
  }
  list(): Observable<App[]> {
    return this.appMap().pipe(map(appMap => Array.from(appMap.values())));
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
    return zip(
      this.appService.getAppList(),
      this.http.get<{ apps: string[] }>(`${this.server}/api/app`),
      this.appStatService.getAppStat(),
    ).pipe(
      map(([dstoreApps, { apps }, statMap]) => {
        const list = dstoreApps.filter(app => apps.includes(app.name)).map((app: App) => {
          const stat = statMap.get(app.name) || new AppStat();
          app.downloads = stat.downloads;
          app.rate = stat.rate;
          app.ratings = stat.votes;
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
    return this.list().pipe(map(apps => apps.filter(app => app.category === category)));
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
