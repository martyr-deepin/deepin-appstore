import { Component, OnInit, Input } from '@angular/core';
import { DomSanitizer } from '@angular/platform-browser';
import { StoreJobType, StoreJobStatus } from '../../models/store-job-info';
import { StoreService } from '../../services/store.service';
import { trigger, transition, style, animate, state } from '@angular/animations';

const animateTime = 250;

@Component({
  selector: 'dstore-progress-button',
  templateUrl: './progress-button.component.html',
  styleUrls: ['./progress-button.component.scss'],
  animations: [
    trigger('fadeLeft', [
      transition(':enter', [
        style({
          transform: 'translateX(-4rem) translateY(-50%)',
          opacity: 0,
        }),
        animate(animateTime),
      ]),
      transition(':leave', [
        animate(
          animateTime,
          style({
            transform: 'translateX(-4rem) translateY(-50%)',
            opacity: 0,
          }),
        ),
      ]),
    ]),

    trigger('fadeRight', [
      transition(':enter', [
        style({
          transform: 'translateX(2rem) translateY(-50%)',
          opacity: 0,
        }),
        animate(animateTime),
      ]),
      transition(':leave', [
        animate(
          animateTime,
          style({
            transform: 'translateX(2rem) translateY(-50%)',
            opacity: 0,
          }),
        ),
      ]),
    ]),
    // trigger('fadeRight', [
    //   transition(':enter', [style({ paddingLeft: '30px' }), animate(1000)]),
    //   transition(':leave', [animate(1000, style({ paddingLeft: '30px' }))]),
    // ]),
  ],
})
export class ProgressButtonComponent implements OnInit {
  constructor(private storeService: StoreService, private sanitizer: DomSanitizer) {}
  StoreJobType = StoreJobType;
  StoreJobStatus = StoreJobStatus;
  @Input() job: string;
  @Input() type: StoreJobType;
  @Input() status: StoreJobStatus;
  @Input() progress: number;
  @Input() disabled: boolean;

  get style() {
    return this.sanitizer.bypassSecurityTrustStyle(
      `--progress:${(this.progress * 100).toFixed()}%`,
    );
  }
  ngOnInit() {}

  click() {
    if (this.status === StoreJobStatus.paused) {
      this.storeService.resumeJob(this.job);
    } else {
      this.storeService.pauseJob(this.job);
    }
  }
}
