import { Injectable } from '@angular/core';
import { StoreService } from 'app/modules/client/services/store.service';
import { bufferTime, filter, share, map, mergeMap, first } from 'rxjs/operators';
import { Subject } from 'rxjs';
import { JobService } from './job.service';

@Injectable({
  providedIn: 'root',
})
export class PackageService {
  constructor(private storeService: StoreService, private jobService: JobService) {}

  private query$ = new Subject<QueryOption>();
  private result$ = this.query$.pipe(
    bufferTime(100, -1, 10),
    filter(arr => arr.length > 0),
    mergeMap(arr => {
      const opt = [...new Map(arr.map(opt => [opt.name, opt] as [string, QueryOption])).values()];
      return this.storeService.query(opt);
    }),
    share(),
  );
  query(opt: QueryOption) {
    setTimeout(() => this.query$.next(opt));
    return this.result$.pipe(
      filter(results => results.has(opt.name)),
      map(results => results.get(opt.name)),
    );
  }
  querys(opts: QueryOption[]) {
    setTimeout(() => opts.forEach(opt => this.query$.next(opt)));
    return this.result$.pipe(map(results => opts.map(opt => results.get(opt.name)).filter(Boolean)));
  }
}

interface QueryOption {
  name: string;
  localName: string;
  packages: { packageURI: string }[];
}
