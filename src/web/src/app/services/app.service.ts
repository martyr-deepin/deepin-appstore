import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, combineLatest, Subject } from 'rxjs';
import { map, scan, share, debounceTime, flatMap } from 'rxjs/operators';

import { BaseService } from '../dstore/services/base.service';
import { AppService as DstoreAppService } from '../dstore/services/app.service';
import { App as DstoreApp } from '../dstore/services/app';
import { AppVersion } from '../dstore-client.module/models/app-version';
import { AppStatService, AppStat } from './stat.service';
import { StoreService } from 'app/dstore-client.module/services/store.service';

@Injectable()
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

  private getAppQuery = new Subject<string>();
  private getAppResult = this.getAppQuery.pipe(
    scan((acc: string[], name: string) => [...acc, name], []),
    debounceTime(10),
    flatMap(list => this.getApps(list)),
    share(),
  );

  getAppMap() {
    return combineLatest(
      this.appService.getAppList(),
      this.activeList,
      this.appStatService.appStatMap,
    ).pipe(
      map(([dstoreApps, { apps }, statMap]) => {
        const list = dstoreApps.filter(app => apps.includes(app.name)).map((app: App) => {
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
    setTimeout(() => {
      this.getAppQuery.next(appName);
    }, 0);
    return this.getAppResult.pipe(map(list => list.find(app => app.name === appName)));
  }
}

export class App extends DstoreApp {
  downloads = 0;
  rate = 0;
  ratings = 0;
  version: AppVersion;
}
