import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class RegionService {
  constructor(private http: HttpClient) {}
  region$ = this.http.get<Region>(environment.metadataServer + '/api/v3/region');
}
interface Region {
  Country: Country;
}

interface Country {
  IsoCode: string;
}
