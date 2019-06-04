import { Injectable } from '@angular/core';
import { HttpClient, HttpResponse } from '@angular/common/http';
import { Resolve, ActivatedRouteSnapshot } from '@angular/router';
import { map, tap, switchMap } from 'rxjs/operators';
import { SoftwareService, Software } from 'app/services/software.service';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class RankingService implements Resolve<ResolveModel[]> {
  constructor(private http: HttpClient, private softService: SoftwareService) {
    console.log(environment);
  }
  readonly url = environment.operationServer + '/api/v3/apps';
  readonly limit = 20;

  async list({
    order = 'download' as 'download' | 'score',
    offset = 0,
    limit = 20,
    category = '',
    tag = '',
  }) {
    const stats = await this.http
      .get<Stat[]>(this.url, {
        params: { order, offset, limit, category, tag } as any,
      })
      .toPromise();
    const softs = await this.softService
      .getSofts(stats.map(v => v.name))
      .toPromise();
    return stats.map(stat => {
      const soft = softs.find(soft => soft.name === stat.name);
      return this.softService.attach(soft, stat);
    });
  }

  asyncIterator({ order = 'download' as 'download' | 'score' }) {
    const asyncList = {
      limit: 20,
      offset: 0,
      total: 0,
      list: this.list({}),
      [Symbol.asyncIterator]: () => ({
        next: async () => {
          const n = asyncList.offset % asyncList.limit;
          if (asyncList.offset >= asyncList.limit && n === 0) {
            asyncList.list = this.list({ order, offset: asyncList.offset });
          }
          const list = await asyncList.list;
          asyncList.offset++;
          const value = list[n];
          return { value, done: !Boolean(value) };
        },
      }),
    };
    return asyncList;
  }

  resolve(route: ActivatedRouteSnapshot) {
    const order: any = route.queryParamMap.get('order') || 'download';
    return this.list({ order });
  }
}

export type ResolveModel = Stat & Software;

export interface Stat {
  name: string;
  score: number;
  score_count: number;
  download: number;
}
interface StatListOption {
  order?: 'download' | 'score';
  offset?: number;
}
