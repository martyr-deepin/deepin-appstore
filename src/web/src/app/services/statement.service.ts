import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class StatementService {
  constructor(private http: HttpClient) {}
  getStatement() {
    const language = navigator.language.replace('-', '_');
    return this.http.get(environment.metadataServer + `/api/v2/agreement`, {
      responseType: 'text',
      params: { language },
    });
  }
}
