import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { parse } from 'marked';

import { catchError, map, switchMap } from 'rxjs/operators';
import { bindCallback, bindNodeCallback } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class StatementService {
  constructor(private http: HttpClient) {}
  getStatement() {
    return this.http
      .get(`/assets/markdowns/Donation.${navigator.language}.md`, { responseType: 'text' })
      .pipe(
        catchError(() => {
          return this.http.get(`/assets/markdowns/Donation.en-US.md`, { responseType: 'text' });
        }),
        switchMap(body => {
          return new Promise<string>((resolve, reject) => {
            parse(body, { breaks: true }, (e, result) => (e ? reject(e) : resolve(result)));
          });
        }),
      );
  }
}
