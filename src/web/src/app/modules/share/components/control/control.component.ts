import { Component, OnInit, Input, Output } from '@angular/core';
import { Software, SoftwareService } from 'app/services/software.service';
import { PackageService } from 'app/services/package.service';
import { Observable, BehaviorSubject, Subject } from 'rxjs';
import { share, map, pairwise, startWith, tap, first } from 'rxjs/operators';
import { JobService } from 'app/services/job.service';
import {
  trigger,
  animate,
  style,
  transition,
  keyframes,
} from '@angular/animations';
import {
  StoreJobInfo,
  StoreJobStatus,
} from 'app/modules/client/models/store-job-info';

@Component({
  selector: 'dstore-control',
  templateUrl: './control.component.html',
  styleUrls: ['./control.component.scss'],
  animations: [
    trigger('circle', [
      transition(
        ':enter',
        animate(
          200,
          keyframes([
            style({ opacity: 0, transform: 'translateX(100%)' }),
            style({ opacity: 1, transform: 'translateX(0)' }),
          ]),
        ),
      ),
      transition(
        ':leave',
        animate(
          200,
          keyframes([
            style({
              position: 'absolute',
              opacity: 1,
              transform: 'translateX(0)',
            }),
            style({
              position: 'absolute',
              opacity: 0,
              transform: 'translateX(100%)',
            }),
          ]),
        ),
      ),
    ]),
    trigger('button', [
      transition(
        ':enter',
        animate(
          200,
          keyframes([
            style({
              opacity: 0.5,
              transform: 'translateX(-50%)',
            }),
            style({
              opacity: 1,
              transform: 'translateX(0)',
            }),
          ]),
        ),
      ),
      transition(
        ':leave',
        animate(
          200,
          keyframes([
            style({
              position: 'absolute',
              opacity: 1,
              transform: 'translateX(0)',
            }),
            style({
              position: 'absolute',
              opacity: 0.5,
              transform: 'translateX(-100%)',
            }),
          ]),
        ),
      ),
    ]),
  ],
})
export class ControlComponent implements OnInit {
  constructor(
    private softwareService: SoftwareService,
    private packageService: PackageService,
    private jobService: JobService,
  ) {}
  @Input() soft: Software;
  package$ = new BehaviorSubject(null);
  @Output() job$: Observable<any>;
  JobStatus = StoreJobStatus;
  show = false;
  ngOnInit() {
    this.package$.subscribe(v => {
      console.log('pcakge ', v);
    });
    this.queryPackage();
    this.job$ = this.jobService.jobsInfo().pipe(
      map(jobs => jobs.find(job => job.names.includes(this.soft.name))),
      startWith(null),
      pairwise(),
      map(([old, job]) => {
        if (job) {
          this.show = true;
        }
        if (old && !job) {
          this.queryPackage();
        }
        return job;
      }),
      share(),
    );
  }

  queryPackage() {
    this.packageService
      .query({
        name: this.soft.name,
        localName: this.soft.info.name,
        packages: this.soft.info.packages,
      })
      .pipe(
        share(),
        first(),
      )
      .subscribe(pkg => this.package$.next(pkg));
  }

  openApp(e: Event) {
    e.stopPropagation();
    this.softwareService.open(this.soft);
  }

  installApp(e: Event) {
    e.stopPropagation();
    this.softwareService.install(this.soft);
  }

  trgger(e: Event, job: StoreJobInfo) {
    e.stopPropagation();
    if (
      job.status === this.JobStatus.paused ||
      job.status === this.JobStatus.failed
    ) {
      job.status = this.JobStatus.running;
      this.jobService.startJob(job.job);
    } else {
      job.status = this.JobStatus.paused;
      this.jobService.stopJob(job.job);
    }
  }
}
