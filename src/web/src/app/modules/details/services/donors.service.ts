import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { BaseService } from 'app/dstore/services/base.service';

@Injectable({
  providedIn: 'root',
})
export class DonorsService {
  apiURL = BaseService.serverHosts.metadataServer + '/api/donation/app/';
  constructor(private http: HttpClient) {}
  getDonation(appName: string) {
    return this.http.get<{ donators: number[]; totalCount: number }>(this.apiURL + appName);
  }
}
