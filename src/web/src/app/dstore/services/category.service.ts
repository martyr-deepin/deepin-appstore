import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { keyBy, first, toArray, throttle, groupBy } from 'lodash';
import { Observable } from 'rxjs';
import { retry, map, shareReplay } from 'rxjs/operators';

import { BaseService } from './base.service';
import { Locale } from '../utils/locale';

@Injectable()
export class CategoryService {
  metadataServer = BaseService.serverHosts.metadataServer;

  constructor(private http: HttpClient) {}

  getList = throttle(this._getList, 60 * 1000);
  private _getList(): Observable<{ [key: string]: Category }> {
    return this.http.get<Category[]>(`${this.metadataServer}/api/category`).pipe(
      retry(3),
      map((categories: Category[]) => {
        const localCategory = groupBy(categories, c => c.Locale);
        return keyBy(
          localCategory[Locale.getUnixLocale()] || first(toArray(localCategory)),
          c => c.Name,
        );
      }),
      shareReplay(),
    );
  }
}

export class Category {
  Locale: string;
  LocalName: string;
  Name: string;
  Active: boolean;
}
