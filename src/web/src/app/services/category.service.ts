import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import * as _ from 'lodash';

import { BaseService } from '../dstore/services/base.service';
import { Category as DstoreCategory } from '../dstore/services/category.service';

@Injectable()
export class CategoryService {
  private _list: Observable<Category[]>;

  constructor(private http: HttpClient, private baseService: BaseService) {
    this._list = this.getList().shareReplay();
  }

  get list() {
    return this._list;
  }

  private getList() {
    return this.http
      .get(`${this.baseService.serverHosts.operationServer}/api/blob/category`)
      .flatMap((ccs: CustomCategory[]) => {
        if (ccs && ccs.length > 10) {
          return Observable.of(
            ccs.map((c, index) => ({
              id: index.toString(),
              title: c.name,
              icon: c.icon,
              apps: c.apps
            }))
          );
        } else {
          return this.http
            .get(`${this.baseService.serverHosts.metadataServer}/api/category`)
            .map((cs: DstoreCategory[]) => {
              return _.chain(cs)
                .keyBy('Name')
                .map((c: DstoreCategory) => ({
                  id: c.Name,
                  title: c.Name,
                  icon: ''
                }))
                .value();
            });
        }
      });
  }
}
export interface Category {
  id: string;
  title: string;
  icon?: string;
  apps?: string[];
}
interface CustomCategory {
  name: '';
  icon: '';
  show: true;
  apps: string[];
}
