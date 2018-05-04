import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { BaseService } from '../dstore/services/base.service';
import { Section } from '../dstore/services/section';
import { Observable } from 'rxjs';

@Injectable()
export class SectionService {
  constructor(private http: HttpClient) {}
  getList(): Observable<Section[]> {
    return this.http
      .get(`${BaseService.serverHosts.operationServer}/api/blob/section`)
      .map((ss: Section[]) => ss);
  }
}
