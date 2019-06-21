import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { first } from 'rxjs/operators';

import { environment } from 'environments/environment';
import { AuthService } from './auth.service';
import { Channel } from 'app/modules/client/utils/channel';
import { Software } from './software.service';

@Injectable({
  providedIn: 'root',
})
export class DownloadTotalService {
  server = environment.operationServer;
  constructor(private http: HttpClient, private auth: AuthService) {}

  installed(apps: Software[]) {
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
  private downloadTotal(app: Software) {
    const url = this.server + '/downloading/app/' + app.name;
    this.http.post(url, null).subscribe();
  }
  // 记录云端应用
  private addUserApps(apps: Software[]) {
    const userApps = apps.map(app => {
      return {
        appName: app.name,
        version: app.package.remoteVersion,
      };
    });
    const url = this.server + '/api/user/my/app/';
    this.http.post(url, userApps).subscribe();
  }
  // 同步安装,记录软件下载统计
  private async installApps(apps: Software[]) {
    const url = this.server + '/api/user/app/install';
    const params = {};
    const auto = await Channel.exec<Boolean>('settings.getAutoInstall');
    if (auto) {
      params['sync'] = 'true';
    }
    const install = apps.map(app => {
      return {
        appName: app.name,
        packageURLs: app.info.packages.map(pkg => pkg.packageURI),
      };
    });
    return this.http.post(url, { install }, { params }).toPromise();
  }
}
