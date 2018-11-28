import { Injectable, NgZone } from '@angular/core';
import { BehaviorSubject, Observable, Subscription, of, timer } from 'rxjs';
import { switchMap, share, map } from 'rxjs/operators';

import { StoreService } from 'app/modules/client/services/store.service';
import { StoreJobInfo } from 'app/modules/client/models/store-job-info';

@Injectable({
  providedIn: 'root',
})
export class JobService {
  private jobList$ = new BehaviorSubject<string[]>([]);
  private jobInfoList$ = new BehaviorSubject<StoreJobInfo[]>([]);
  private interval: Subscription;
  private cache = new Map<string, StoreJobInfo>();
  constructor(private zone: NgZone, private StoreServer: StoreService) {
    this.StoreServer.getJobList().subscribe(list => this.update(list));
    this.StoreServer.jobListChange().subscribe(list => this.update(list));
  }
  private update(list: string[]) {
    this.zone.run(() => {
      this.jobList$.next(list);
      if (this.interval) {
        this.interval.unsubscribe();
      }
      const defer = Array.from(this.cache.values())
        .filter(job => !list.includes(job.job))
        .map(job => job.id);
      if (defer.length > 0) {
        setTimeout(() => {
          defer.forEach(id => this.cache.delete(id));
          this.jobInfoList$.next(Array.from(this.cache.values()));
        }, 500);
      }
      if (list.length > 0) {
        this.interval = timer(0, 1000)
          .pipe(switchMap(() => this.StoreServer.getJobsInfo(list)))
          .subscribe(infoList => {
            infoList.forEach(job => {
              this.cache.set(job.id, job);
            });
            this.jobInfoList$.next(Array.from(this.cache.values()));
          });
      } else {
        this.jobInfoList$.next(Array.from(this.cache.values()));
      }
    });
  }
  jobList(): Observable<string[]> {
    return this.jobList$.asObservable();
  }
  jobsInfo(): Observable<StoreJobInfo[]> {
    return this.jobInfoList$.asObservable();
  }
}
