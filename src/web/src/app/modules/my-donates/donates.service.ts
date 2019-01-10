import { AppService } from 'app/services/app.service';
import { switchMap } from 'rxjs/operators';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'environments/environment';
import { DstoreApp, RawApp } from 'app/model';

@Injectable({
  providedIn: 'root',
})
export class DonatesService {
  server = environment.metadataServer;
  apiURL = environment.metadataServer + '/api/v2/user/donation';
  constructor(private http: HttpClient, private appService: AppService) {}
  donateList(page: number, count: number) {
    const split = `${(page - 1) * count}:${page * count}`;
    return this.http.get<Result>(this.apiURL, { params: { split } }).pipe(
      switchMap(
        result => {
          const names = result.donations.map(d => d.appName);
          return this.appService.getApps(names, false, false);
        },
        (result, apps) => {
          const deletedApps = !result.apps
            ? []
            : result.apps.map(app => {
                app = app as RawApp;
                const dstoreApp = new DstoreApp(app);
                dstoreApp.icon =
                  environment.metadataServer + '/images/' + dstoreApp.icon;
                return dstoreApp;
              });
          result.donations = result.donations.map(d => {
            d = this.appService.addApp(
              d,
              apps.find(app => app.name === d.appName),
            );
            const dApp = deletedApps.find(
              app => app.name === d.appName,
            ) as DstoreApp;
            if (dApp) {
              d = dApp.AttachTo(d);
            }
            return d;
          });
          return result;
        },
      ),
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
