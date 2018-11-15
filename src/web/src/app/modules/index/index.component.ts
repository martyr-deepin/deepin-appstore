import { Component, OnInit, OnDestroy } from '@angular/core';

import { Observable, Subscription, merge, timer, of } from 'rxjs';
import { map, tap, flatMap, shareReplay, switchMap, concat, startWith } from 'rxjs/operators';

import { SectionService } from './services/section.service';
import { Section, SectionType } from 'app/dstore/services/section';
import { App } from 'app/services/app.service';
import { AppService } from 'app/services/app.service';
import { StoreService } from 'app/modules/client/services/store.service';
import { StoreJobInfo } from 'app/modules/client/models/store-job-info';

@Component({
  selector: 'app-index',
  templateUrl: './index.component.html',
  styleUrls: ['./index.component.scss'],
})
export class IndexComponent implements OnInit, OnDestroy {
  constructor(
    private sectionService: SectionService,
    private appService: AppService,
    private storeService: StoreService,
  ) {}
  SectionType = SectionType;
  sectionList$: Observable<Section[]>;
  appMap: Map<string, App>;

  // job
  jobs: { [key: string]: StoreJobInfo } = {};
  jobsNames = new Set<string>();
  jobs$: Subscription;

  ngOnInit() {
    this.appService.getAppMap().subscribe(appMap => {
      this.appMap = appMap;
      this.sectionList$ = this.sectionService.getList().pipe(map(ss => ss.filter(s => s.show)));
    });

    this.jobs$ = this.getJobs();
  }
  ngOnDestroy() {
    this.jobs$.unsubscribe();
  }

  getJobs() {
    return merge(this.storeService.getJobList(), this.storeService.jobListChange())
      .pipe(
        switchMap(jobs => {
          if (jobs.length > 0) {
            return timer(0, 1000).pipe(flatMap(() => this.storeService.getJobsInfo(jobs)));
          } else {
            return of([] as StoreJobInfo[]);
          }
        }),
      )
      .subscribe(jobInfos => {
        const jobs: { [key: string]: StoreJobInfo } = {};
        jobInfos.forEach(job => {
          job.names.forEach(name => {
            jobs[name] = job;
            this.jobsNames.add(name);
          });
        });
        this.jobs = jobs;
      });
  }
}
