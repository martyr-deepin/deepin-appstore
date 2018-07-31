import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { BehaviorSubject, Observable, forkJoin, Subject } from 'rxjs';
import { map, first, throttleTime, exhaustMap, catchError, take } from 'rxjs/operators';
import { BaseService } from '../dstore/services/base.service';

@Injectable({
  providedIn: 'root',
})
export class AppStatService {
  private apiURL = BaseService.serverHosts.operationServer + '/api/appstat';
  private sync$ = new Subject<string>();
  private appStatMap$ = new BehaviorSubject<Map<string, AppStat>>(new Map());
  constructor(private http: HttpClient) {
    this.startSync();
  }

  getAppStat(): Observable<Map<string, AppStat>>;
  getAppStat(name: string): Observable<AppStat>;

  getAppStat(name?: string) {
    if (name) {
      this.sync$.next(name);
      return this.appStatMap$.pipe(map(asm => asm.get(name) || new AppStat()));
    }
    this.sync$.next(null);
    this.http.get<Result>(this.apiURL).subscribe(result => {
      result.downloadCount.forEach(dc => {});
    });
    return this.appStatMap$.pipe(take(2));
  }
  startSync() {
    this.sync$
      .pipe(
        throttleTime(1000),
        exhaustMap(name => {
          const params = name ? { apps: name } : null;
          return forkJoin(
            this.appStatMap$.pipe(first()),
            this.http.get<Result>(this.apiURL, { params }),
          );
        }),
        catchError((err, caught) => {
          console.log(err);
          return caught;
        }),
      )
      .subscribe(([asm, result]) => {
        result.downloadCount.forEach(dc => {
          if (asm.has(dc.appName)) {
            asm.get(dc.appName).downloads = dc.count;
          } else {
            const stat = new AppStat();
            stat.downloads = dc.count;
            asm.set(dc.appName, stat);
          }
        });
        result.rate.forEach(rt => {
          if (asm.has(rt.appName)) {
            const stat = asm.get(rt.appName);
            stat.rate = rt.rate;
            stat.votes = rt.count;
          } else {
            const stat = new AppStat();
            stat.rate = rt.rate;
            stat.votes = rt.count;
            asm.set(rt.appName, stat);
          }
        });
        console.log(asm);
        this.appStatMap$.next(asm);
      });
  }
}

export class AppStat {
  downloads: number;
  rate: number;
  votes: number;
  constructor() {
    return { downloads: 0, rate: 0, votes: 0 };
  }
}

interface Result {
  downloadCount: DownloadCount[];
  rate: Rate[];
}

interface Rate {
  appName: string;
  rate: number;
  count: number;
}

interface DownloadCount {
  appName: string;
  count: number;
}
