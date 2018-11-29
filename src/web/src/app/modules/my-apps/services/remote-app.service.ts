import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { switchMap } from 'rxjs/operators';

import { environment } from 'environments/environment';
import { App, AppService } from 'app/services/app.service';

@Injectable({
  providedIn: 'root',
})
export class RemoteAppService {
  url = environment.operationServer + '/api/my/app';
  constructor(private http: HttpClient, private appService: AppService) {}
  RemoteAppList(page: number, pageSize: number) {
    const params = {
      page: page.toString(),
      count: pageSize.toString(),
    };
    return this.http.get<Result>(this.url, { params }).pipe(
      switchMap(
        result => this.appService.getApps(result.apps.map(app => app.appName), false),
        (result, apps) => {
          result.apps.forEach(remoteApp => {
            remoteApp.app = apps.find(app => app.name === remoteApp.appName);
          });
          return result;
        },
      ),
    );
  }
}

interface Result {
  apps: RemoteApp[];
  totalCount: number;
}
interface RemoteApp {
  appName: string;
  time: Date;
  app: App;
}
