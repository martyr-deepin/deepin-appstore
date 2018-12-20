import { App } from './../../services/app.service';
import { AppService } from 'app/services/app.service';
import { switchMap } from 'rxjs/operators';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class DonatesService {
  apiURL = environment.metadataServer + '/api/donation/user';
  constructor(private http: HttpClient, private appService: AppService) {}
  donateList(page: number, count: number) {
    return this.http
      .get<Result>(this.apiURL, {
        params: { page: page.toString(), count: count.toString() },
      })
      .pipe(
        switchMap(
          result => {
            const names = result.donations.map(d => d.appName);
            return this.appService.getApps(names, false, false);
          },
          (result, apps) => {
            result.donations.forEach(d => {
              d.app = apps.find(app => app.name === d.appName);
            });
            return result;
          },
        ),
      );
  }
}

interface Result {
  donations: Donation[];
  totalCount: number;
}

interface Donation {
  app: App;
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
