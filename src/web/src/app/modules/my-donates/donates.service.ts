import { map } from 'rxjs/operators';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'environments/environment';
import { DstoreApp, RawApp } from 'app/model';
import { SoftwareService } from 'app/services/software.service';

@Injectable({
  providedIn: 'root',
})
export class DonatesService {
  server = environment.metadataServer;
  apiURL = environment.metadataServer + '/api/v2/user/donation';
  constructor(private http: HttpClient, private softService: SoftwareService) {}
  donateList(page: number, count: number) {
    const split = `${(page - 1) * count}:${page * count}`;
    return this.http.get<Result>(this.apiURL, { params: { split } }).pipe(
      map(result => {
        result.donations = result.donations.map(d => {
          const app = result.apps.find(app => app.name === d.appName);
          if (app) {
            new DstoreApp(app as RawApp).AttachTo(d);
          }
          return d;
        });
        console.log(result);
        return result;
      }),
    );
  }
}

interface Result {
  apps: (RawApp | DstoreApp)[];
  donations: Donation[];
  total: number;
}

interface Donation {
  id: number;
  createTime: string;
  donator: number;
  appName: string;
  amount: number;
  currency: string;
  exchange_rate: number;
  actualAmount: number;
  source: number;
  tradeID: string;
  appStore: string;
}
