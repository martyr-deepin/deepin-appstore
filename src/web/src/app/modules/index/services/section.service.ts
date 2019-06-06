import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { environment } from 'environments/environment';
import { Section } from '../../../dstore/services/section';
import { map, shareReplay } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class SectionService {
  url = `${environment.operationServer}/api/blob/section`;
  list = this.http
    .get<Section[]>(this.url)
    .pipe(
      map(ss => ss || []),
      map(ss => ss.filter(s => s.show)),
    )
    .toPromise();
  constructor(private http: HttpClient) {}
  getList() {
    return this.list;
  }
}
