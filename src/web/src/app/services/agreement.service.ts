import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';
import { switchMap } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class AgreementService {
  constructor(private http: HttpClient) {}
  private readonly baseURL = environment.operationServer + '/api/blob';
  privacy() {
    return this.http
      .get<Agreement>(this.baseURL + '/privacy')
      .pipe(
        switchMap(agreement => fetch(agreement[environment.locale] || agreement['en_US']).then(resp => resp.text())),
      );
  }
  donation() {
    return this.http
      .get<Agreement>(this.baseURL + '/donation')
      .pipe(
        switchMap(agreement => fetch(agreement[environment.locale] || agreement['en_US']).then(resp => resp.text())),
      );
  }
}

interface Agreement {
  [key: string]: string;
}
