import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';
import { map } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class RegionService {
  constructor(private http: HttpClient) {}
  region$ = this.http
    .get<Region>(environment.metadataServer + '/api/v3/region')
    .pipe(map(region => region.Country.IsoCode));
}
interface Region {
  Country: {
    IsoCode: string;
  };
}
