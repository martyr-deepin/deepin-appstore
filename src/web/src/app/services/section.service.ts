import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { environment } from 'environments/environment';
import { Section } from '../dstore/services/section';
import { map, shareReplay } from 'rxjs/operators';

@Injectable()
export class SectionService {
  url = `${environment.operationServer}/api/blob/section`;
  list = this.http.get(this.url).pipe(
    map((ss: Section[]) => ss),
    shareReplay(),
  );
  constructor(private http: HttpClient) {}
  getList() {
    return this.list;
  }
}
