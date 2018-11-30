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
  list = this.http.get<Section[]>(this.url).pipe(
    map(ss => {
      return ss || [];
    }),
    shareReplay(1),
  );
  constructor(private http: HttpClient) {}
  getList() {
    return this.list;
  }
}
