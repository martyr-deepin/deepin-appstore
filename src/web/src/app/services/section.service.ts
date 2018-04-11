import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { BaseService } from '../dstore/services/base.service';
import { Section } from '../dstore/services/section';
import { Observable } from 'rxjs/Observable';

@Injectable()
export class SectionService {
  constructor(private http: HttpClient, private baseService: BaseService) {}
  getList(): Observable<Section[]> {
    return this.http
      .get(`${this.baseService.serverHosts.operationServer}/api/blob/section`)
      .map((ss: Section[]) => ss);
  }
}
