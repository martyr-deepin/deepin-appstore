import { Injectable } from '@angular/core';
import { HttpClient, HttpResponse } from '@angular/common/http';
import { Resolve, ActivatedRouteSnapshot } from '@angular/router';
import { map, tap } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class RankingService implements Resolve<any> {
  constructor(private http: HttpClient) {}
  list(option: {
    order?: 'download' | 'score';
    offset?: number;
    limit?: number;
  }) {
    if (!option.order) {
      option.order = 'download';
    }
    const url = 'http://127.0.0.1:8100/api/v3/apps';
    return this.http
      .get<App[]>(url, { observe: 'response', params: option as any })
      .pipe(
        map(ResponseToResouceList),
        tap(list => {
          list.start = option.offset || 0;
          list.end = list.start + (option.limit || 10);
        }),
      );
  }
  resolve(route: ActivatedRouteSnapshot) {
    const order: any = route.queryParamMap.get('order') || 'download';
    return this.list({ order, limit: 20 });
    console.log(route);
  }
}

export interface App {
  name: string;
  score: number;
  score_count: number;
  download: number;
}

export class ResourceList<T> extends Array<T> {
  total: number;
  start: number;
  end: number;
  constructor(...items: T[]) {
    super(...items);
  }
  append(list: ResourceList<T>) {
    list.unshift(...this);
    return list;
  }
}
function ResponseToResouceList<T>(resp: HttpResponse<T[]>) {
  const list = new ResourceList(...resp.body);
  list.total = Number(resp.headers.get('x-resource-total'));
  return list;
}
