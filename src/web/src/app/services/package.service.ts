import { Injectable } from '@angular/core';
import { StoreService } from 'app/modules/client/services/store.service';
import { bufferTime, filter, share, map, mergeMap, first, tap } from 'rxjs/operators';
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

  _query(opt: QueryOption) {
    return this.result$.pipe(
      filter(results => results.has(opt.name)),
      map(results => results.get(opt.name)),
    );
  }

  query(opt: QueryOption) {
    setTimeout(() => this.query$.next(opt));
    return this._query(opt);
  }

  async querys(opts: QueryOption[]) {
    setTimeout(() => opts.forEach(opt => this.query$.next(opt)));
    const list = await Promise.all(
      opts.map(opt =>
        this._query(opt)
          .pipe(first())
          .toPromise(),
      ),
    );
    return new Map(list.filter(pkg => pkg && pkg.remoteVersion).map(pkg => [pkg.appName, pkg]));
  }
}

interface QueryOption {
  name: string;
  localName: string;
  packages: { packageURI: string }[];
}
