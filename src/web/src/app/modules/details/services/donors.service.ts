import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class DonorsService {
  apiURL = environment.metadataServer + '/api/donation/app/';
  constructor(private http: HttpClient) {}
  getDonation(appName: string) {
    return this.http.get<{ donators: number[]; totalCount: number }>(this.apiURL + appName);
  }
}
