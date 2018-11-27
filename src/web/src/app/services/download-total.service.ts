import { AuthService } from './auth.service';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class DownloadTotalService {
  private downloadTotalUrl = environment.operationServer + '/api/downloading/app/';
  private remoteAppUrl = environment.operationServer + '/api/my/app/';
  constructor(private http: HttpClient, private auth: AuthService) {}
  // 下载统计和记录我的云端应用
  installed(appName: string) {
    this.http.post(this.downloadTotalUrl, null).subscribe();
    this.auth.logged$.subscribe(logged => {
      if (logged) {
        this.http.put(this.remoteAppUrl + appName, null).subscribe();
      }
    });
  }
}
