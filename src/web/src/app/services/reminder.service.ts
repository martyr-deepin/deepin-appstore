import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class ReminderService {
  constructor(private http: HttpClient) {}
  api = `${environment.operationServer}/api/user/updating/`;
  reminder(appName: string, version: string): Observable<void> {
    return this.http.post<void>(this.api + appName, {
      appName,
      version,
    });
  }
}
