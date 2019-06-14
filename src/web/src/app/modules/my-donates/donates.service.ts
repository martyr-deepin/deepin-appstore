import { map, switchMap } from 'rxjs/operators';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'environments/environment';
import { DstoreApp, RawApp } from 'app/model';
import { SoftwareService, Software } from 'app/services/software.service';

@Injectable({
  providedIn: 'root',
})
export class DonatesService {
  server = environment.metadataServer;
  apiURL = environment.metadataServer + '/api/v3/user/donation';
  constructor(private http: HttpClient, private softService: SoftwareService) {}
  donateList(page: number, count: number) {
    const params = { offset: (page - 1) * count, limit: count };
    return this.http.get<Donation[]>(this.apiURL, { observe: 'response', params: params as any }).pipe(
      switchMap(async resp => {
        const donations = resp.body;
        const list = await this.softService.list({
          names: donations.map(d => d.appName),
          filterPackage: false,
          filterStat: false,
        });
        const map = new Map(list.map(soft => [soft.name, soft]));
        const result = {
          total_count: Number(resp.headers.get('x-total-count')),
          amount_count: Number(resp.headers.get('x-amount-count')),
          exchange_rate: Number(resp.headers.get('x-exchange-rate')),
          donations: donations.map(d => {
            d.soft = map.get(d.appName);
            return d;
          }),
        };
        return result;
      }),
    );
  }
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

  soft: Software;
}
