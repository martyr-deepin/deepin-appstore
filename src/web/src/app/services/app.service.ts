import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { get, keyBy, throttle, forEach } from 'lodash';
import { Observable, forkJoin } from 'rxjs';
import { map, flatMap, tap, share, shareReplay, retry } from 'rxjs/operators';

import { BaseService } from '../dstore/services/base.service';
import { AppService as DstoreAppService } from '../dstore/services/app.service';
import { App as DstoreApp } from '../dstore/services/app';

@Injectable()
export class AppService {
  constructor(private http: HttpClient, private appService: DstoreAppService) {}

  private server = BaseService.serverHosts.operationServer;

  list = throttle(this.getList, 1000 * 10);
  private getList() {
    console.log('getList');
    interface AppStat {
      downloadCount: { appName: string; count: number }[];
      rate: { appName: string; rate: number; count: number }[];
    }

    return forkJoin(
      this.http.get<{ apps: string[] }>(`${this.server}/api/app`),
      this.appService.getAppList(),
      this.http.get<AppStat>(`${this.server}/api/appstat`),
    ).pipe(
      retry(3),
      map(([activeList, appList, stat]) => {
        console.log('map');
        const downloadMap = keyBy(stat.downloadCount, 'appName');
        const rateMap = keyBy(stat.rate, 'appName');
        return forEach(<App[]>appList.filter(app => activeList.apps.includes(app.name)), app => {
          app.downloads = get(downloadMap, [app.name, 'count'], 0);
          app.rate = get(rateMap, [app.name, 'rate']) / 2 || 0;
          app.ratings = get(rateMap, [app.name, 'count'], 0);
        });
      }),
      shareReplay(),
    );
  }
  getApp(appName: string): Observable<App> {
    return this.list().pipe(map(apps => apps.find(app => app.name === appName)));
  }
}

export class App extends DstoreApp {
  downloads = 0;
  rate = 0;
  ratings = 0;
}
