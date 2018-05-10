import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, of, concat } from 'rxjs';
import { map, share, retry, tap, shareReplay } from 'rxjs/operators';
import { throttle } from 'lodash';

import { BaseService } from '../dstore/services/base.service';
import { Category as DstoreCategory } from '../dstore/services/category.service';

@Injectable()
export class CategoryService {
  constructor(private http: HttpClient) {}

  list = throttle(this.getList, 1000 * 60);
  private getList(): Observable<Category[]> {
    return concat<Category[]>(
      of(defaultCategory),
      this.http
        .get<CustomCategory[]>(`${BaseService.serverHosts.operationServer}/api/blob/category`)
        .pipe(
          retry(3),
          map(ccs =>
            ccs.filter(c => c.show).map((c, index) => ({
              id: index.toString(),
              title: c.name,
              icon: c.icon,
              apps: c.apps,
            })),
          ),
        ),
    ).pipe(
      map(cs => (cs.length === 0 ? defaultCategory : cs)),
      tap(() => console.log('ok')),
      shareReplay(),
    );
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

const defaultCategory = [
  { id: 'internet', title: 'internet', icon: '' },
  { id: 'office', title: 'office', icon: '' },
  { id: 'development', title: 'development', icon: '' },
  { id: 'reading', title: 'reading', icon: '' },
  { id: 'graphics', title: 'graphics', icon: '' },
  { id: 'game', title: 'game', icon: '' },
  { id: 'music', title: 'music', icon: '' },
  { id: 'system', title: 'system', icon: '' },
  { id: 'video', title: 'video', icon: '' },
  { id: 'chat', title: 'chat', icon: '' },
  { id: 'others', title: 'others', icon: '' },
];
