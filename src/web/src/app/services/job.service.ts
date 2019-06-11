import { Injectable } from '@angular/core';
import { BehaviorSubject, Observable, Subscription, of, timer } from 'rxjs';
import { switchMap, debounceTime } from 'rxjs/operators';

import { StoreService } from 'app/modules/client/services/store.service';
import {
  StoreJobInfo,
  StoreJobType,
  StoreJobStatus,
  StoreJobError,
  StoreJobErrorType,
  CanFixError,
} from 'app/modules/client/models/store-job-info';

@Injectable({
  providedIn: 'root',
})
export class JobService {
  private jobList$ = new BehaviorSubject<string[]>([]);
  private jobInfoList$ = new BehaviorSubject<StoreJobInfo[]>([]);
  private interval: Subscription;
  private cache = new Map<string, StoreJobInfo>();
  constructor(private storeService: StoreService) {
    this.storeService
      .getJobList()
      .pipe(debounceTime(100))
      .subscribe(list => this.update(list));
    this.storeService.jobListChange().subscribe(list => this.update(list));
  }

  private update(list: string[]) {
    this.jobList$.next(list);
    if (this.interval) {
      this.interval.unsubscribe();
    }
    const defer = Array.from(this.cache.values())
      .filter(job => !list.includes(job.job))
      .map(job => job.job);
    if (defer.length > 0) {
      setTimeout(() => {
        defer.forEach(id => this.cache.delete(id));
        this.jobInfoList$.next(Array.from(this.cache.values()));
      }, 500);
    }
    if (list.length > 0) {
      this.interval = timer(0, 1000)
        .pipe(switchMap(() => this.storeService.getJobsInfo(list)))
        .subscribe(infoList => {
          infoList = infoList.filter(job => {
            if (job.type === StoreJobType.uninstall && job.status === StoreJobStatus.failed) {
              return false;
            }
            return true;
          });
          infoList.forEach(job => {
            this.cache.set(job.job, job);
          });
          this.jobInfoList$.next(Array.from(this.cache.values()));
        });
    } else {
      this.jobInfoList$.next(Array.from(this.cache.values()));
    }
  }

  jobList(): Observable<string[]> {
    return this.jobList$.asObservable();
  }

  jobsInfo(): Observable<StoreJobInfo[]> {
    return this.jobInfoList$.asObservable();
  }

  stopJob(jobID: string) {
    this.storeService.pauseJob(jobID);
  }
  startJob(jobID: string) {
    this.storeService.resumeJob(jobID);
  }
  clearJob(jobID: string) {
    this.storeService.clearJob(jobID);
  }
}
