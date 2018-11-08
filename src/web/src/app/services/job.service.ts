import { Injectable, NgZone } from '@angular/core';
import { BehaviorSubject, Observable, Subscription, of, timer } from 'rxjs';
import { switchMap, share, map } from 'rxjs/operators';

import { StoreService } from 'app/dstore-client.module/services/store.service';
import { StoreJobInfo } from 'app/dstore-client.module/models/store-job-info';

@Injectable({
  providedIn: 'root',
})
export class JobService {
  private jobList$ = new BehaviorSubject<string[]>([]);
  private jobInfoList$ = new BehaviorSubject<StoreJobInfo[]>([]);
  private interval: Subscription;
  constructor(private zone: NgZone, private StoreServer: StoreService) {
    this.StoreServer.getJobList().subscribe(list => this.update(list));
    this.StoreServer.jobListChange().subscribe(list => this.update(list));
  }
  private update(list: string[]) {
    this.zone.run(() => {
      this.jobList$.next(list);
      if (list.length > 0) {
        if (this.interval) {
          this.interval.unsubscribe();
        }
        this.interval = timer(0, 1000)
          .pipe(switchMap(() => this.StoreServer.getJobsInfo(list)))
          .subscribe(infoList => this.jobInfoList$.next(infoList));
      } else {
        this.jobInfoList$.next([]);
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
