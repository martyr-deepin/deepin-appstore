import { Component, OnInit, OnDestroy } from '@angular/core';

import { Observable, Subscription, merge, timer, of } from 'rxjs';
import { map, tap, flatMap, shareReplay, switchMap, concat, startWith } from 'rxjs/operators';

import { SectionService } from '../../services/section.service';
import { Section, SectionType } from '../../dstore/services/section';
import { App } from '../../services/app.service';
import { AppService } from '../../services/app.service';
import { StoreService } from '../../dstore-client.module/services/store.service';
import { StoreJobInfo } from '../../dstore-client.module/models/store-job-info';

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
  appFilter = this._appFilter.bind(this);

  // job
  jobs: { [key: string]: StoreJobInfo } = {};
  jobsNames = new Set<string>();
  jobs$: Subscription;

  ngOnInit() {
    this.appService.appMap().subscribe(appMap => {
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

  _appFilter(appName: string): boolean {
    return this.appMap.has(appName);
  }
}
