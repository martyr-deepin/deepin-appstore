import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { first } from 'rxjs/operators';

import { environment } from 'environments/environment';
import { App } from 'app/services/app.service';
import { AuthService } from './auth.service';

@Injectable({
  providedIn: 'root',
})
export class DownloadTotalService {
  server = environment.operationServer;
  constructor(private http: HttpClient, private auth: AuthService) {}

  installed(apps: App[]) {
    if (!Array.isArray(apps) || !apps.length) {
      return;
    }
    this.auth.logged$.pipe(first()).subscribe(logged => {
      if (!logged) {
        this.downloadTotal(apps[0]);
        return;
      }
      this.addUserApps(apps);
      this.installApps(apps);
    });
  }
  // 访客下载统计
  private downloadTotal(app: App) {
    const url = this.server + '/downloading/app/' + app.name;
    this.http.post(url, null).subscribe();
  }
  // 记录云端应用
  private addUserApps(apps: App[]) {
    const userApps = apps.map(app => {
      return {
        appName: app.name,
        version: app.version.remoteVersion,
      };
    });
    const url = this.server + '/api/user/my/app/';
    this.http.post(url, userApps).subscribe();
  }
  // 同步安装
  private installApps(apps: App[]) {
    const url = this.server + '/api/user/app/install';
    const install = apps.map(app => {
      return {
        appName: app.name,
        packageURLs: app.packageURI,
      };
    });
    this.http.post(url, { install }).subscribe();
  }
}
