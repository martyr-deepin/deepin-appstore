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
        const url = this.server + '/downloading/app/' + apps[0].name;
        this.http.post(url, null).subscribe();
        return;
      }
      if (apps.length === 1) {
        const url = this.server + '/api/my/app/' + apps[0].name;
        this.http.put(url, null).subscribe();
      }
      const apiURL = this.server + '/api/user/app/install';
      const install = apps.map(app => {
        return {
          appName: app.name,
          packageURLs: app.packageURI,
        };
      });
      this.http.post(apiURL, { install }).subscribe();
    });
  }
}
