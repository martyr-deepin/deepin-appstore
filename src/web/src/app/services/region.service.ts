import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';
import { map } from 'rxjs/operators';

import { get } from 'lodash';

@Injectable({
  providedIn: 'root',
})
export class RegionService {
  constructor(private http: HttpClient) {}
  region() {
    return this.http
      .get<Region>(environment.metadataServer + '/api/v3/region')
      .pipe(map(region => get(region, 'Country.IsoCode', 'CN')));
  }
}
interface Region {
  Country: {
    IsoCode: string;
  };
}
