import { Injectable } from '@angular/core';
import { HttpClient, HttpResponse } from '@angular/common/http';
import { Resolve, ActivatedRouteSnapshot } from '@angular/router';
import { map, tap, switchMap } from 'rxjs/operators';
import { SoftwareService, Software } from 'app/services/software.service';

@Injectable({
  providedIn: 'root',
})
export class RankingService implements Resolve<ResolveModel[]> {
  constructor(private http: HttpClient, private softService: SoftwareService) {}
  readonly url = 'http://10.0.10.70:18100/api/v3/apps';
  readonly limit = 20;

  async list(option: StatListOption) {
    const stats = await this.http
      .get<Stat[]>(this.url, { params: { ...(option as any), limit: 20 } })
      .toPromise();
    const softs = await this.softService
      .getSofts(stats.map(v => v.name))
      .toPromise();
    return stats.map(stat => {
      const soft = softs.find(soft => soft.name === stat.name);
      return this.softService.attach(soft, stat);
    });
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
