import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

import { BaseService } from '../dstore/services/base.service';

@Injectable({
  providedIn: 'root',
})
export class ReminderService {
  constructor(private http: HttpClient) {}
  server = BaseService.serverHosts.operationServer;
  reminder(appName: string, version: string): Observable<void> {
    return this.http.post<void>(`${this.server}/api/updating`, { appName, version });
  }
}
