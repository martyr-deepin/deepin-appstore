import { JobService } from 'app/services/job.service';
import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, combineLatest, Subject } from 'rxjs';
import { map, scan, share, debounceTime, switchMap, shareReplay, tap } from 'rxjs/operators';

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
    private jobService: JobService,
  ) {}
  private client = Boolean(window['dstore']);
  private server = BaseService.serverHosts.operationServer;

  private activeList = this.http
    .get<{ apps: string[] }>(`${this.server}/api/app`)
    .toPromise()
    .then(result => result.apps || []);

  private appMap$ = combineLatest(
    this.appService.getAppList(),
    this.appStatService.getAppStat(),
    this.activeList,
  ).pipe(
    map(([apps, stats, actives]) => {
      return new Map(
        apps.map((app: App) => {
          const stat = stats.get(app.name) || new AppStat();
          app.downloads = stat.downloads;
          app.rate = stat.rate;
          app.ratings = stat.votes;
          app.active = actives.includes(app.name);
          return [app.name, app] as [string, App];
        }),
      );
    }),
    shareReplay(1),
  );

  getAppMap() {
    return this.appMap$;
  }

  list(): Observable<App[]> {
    return this.getAppMap().pipe(map(appMap => [...appMap.values()]));
  }

  // 根据分类获取应用列表
  getAppListByCategory(category: string): Observable<App[]> {
    return this.list().pipe(
      map(apps => apps.filter(app => app.category === category).map(app => app.name)),
      switchMap(appNameList => this.getApps(appNameList)),
    );
  }

  getApps(appNameList: string[], filterVersion = true, filterActive = true): Observable<App[]> {
    const appMap$ = this.getAppMap();

    if (!this.client) {
      return appMap$.pipe(
        map(appMap => {
          return appNameList
            .map(name => appMap.get(name))
            .filter(app => {
              if (!app) {
                return false;
              }
              if (filterActive && !app.active) {
                return false;
              }
              return true;
            });
        }),
      );
    }

    return appMap$.pipe(
      map(appMap =>
        appNameList
          .map(name => appMap.get(name))
          .filter(app => {
            if (!app) {
              return false;
            }
            if (filterActive && !app.active) {
              return false;
            }
            return true;
          }),
      ),
      switchMap(() => this.jobService.jobList().pipe(debounceTime(100)), apps => apps),
      switchMap(
        apps => this.storeService.queryPackage(apps),
        (apps, pkgMap) => {
          apps.forEach(app => {
            const pkg = pkgMap.get(app.name);
            if (pkg) {
              app.version = {
                name: app.name,
                ...pkg,
              };
            }
          });
          if (filterVersion) {
            return apps.filter(app => Boolean(app.version));
          }
          return apps;
        },
      ),
    );
  }

  getApp(appName: string, filterVersion = true, filterActive = true): Observable<App> {
    return this.getApps([appName], filterVersion, filterActive).pipe(map(apps => apps[0]));
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

export interface App extends DstoreApp {
  downloads: number;
  rate: number;
  ratings: number;
  active: boolean;
  version: AppVersion;
}
