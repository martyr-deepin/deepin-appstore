import { Injectable } from '@angular/core';
import { BaseService } from '../dstore/services/base.service';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root',
})
export class DownloadService {
  constructor(private http: HttpClient) {}
  server = BaseService.serverHosts.operationServer;
  record(appName: string) {
    return this.http.post<void>(`${this.server}/api/downloading/app/${appName}`, null);
  }
}
