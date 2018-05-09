import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { get, keyBy, throttle } from 'lodash';
import { Observable } from 'rxjs';
import { map, flatMap, shareReplay, tap } from 'rxjs/operators';

import { BaseService } from '../dstore/services/base.service';
import { AppService as DstoreAppService } from '../dstore/services/app.service';
import { App as DstoreApp } from '../dstore/services/app';

@Injectable()
export class AppService {
  private server = BaseService.serverHosts.operationServer;
  list = throttle(this.getList, 5000);

  constructor(private http: HttpClient, private appService: DstoreAppService) {}

  getApp(appName: string): Observable<App> {
    return this.list().pipe(map(apps => apps.find(app => app.name === appName)));
  }

  private getList() {
    interface AppStat {
      downloadCount: { appName: string; count: number }[];
      rate: { appName: string; rate: number; count: number }[];
    }
    return this.http.get<{ apps: string[] }>(`${this.server}/api/app`).pipe(
      flatMap(resp => {
        return this.appService
          .getAppList()
          .map(appList => appList.filter(app => resp.apps.includes(app.name)));
      }),
      flatMap((apps: App[]) =>
        this.http.get(`${this.server}/api/appstat`).map((stat: AppStat) => {
          const downloadsDict = keyBy(stat.downloadCount, 'appName');
          const rateDict = keyBy(stat.rate, 'appName');
          apps.forEach(app => {
            app.downloads = get(downloadsDict, [app.name, 'count'], 0) as number;
            app.rate = (get(rateDict, [app.name, 'rate'], 0) as number) / 2;
            app.ratings = get(rateDict, [app.name, 'count'], 0) as number;
          });
          return apps;
        }),
      ),
      shareReplay(),
    );
  }
}

export class App extends DstoreApp {
  downloads = 0;
  rate = 0;
  ratings = 0;
}
