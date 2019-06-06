import { Component, OnInit } from '@angular/core';
import { trigger, transition, animate, keyframes, style, state } from '@angular/animations';
import { Observable, Subject, timer } from 'rxjs';
import { throttleTime, switchMap, startWith, map } from 'rxjs/operators';

import { SectionItemBase } from '../section-item-base';
import { SectionCarousel, CarouselType } from 'app/dstore/services/section';
import { SoftwareService } from 'app/services/software.service';

const timings = 500;

@Component({
  selector: 'index-carousel',
  templateUrl: './carousel.component.html',
  styleUrls: ['./carousel.component.scss'],
  animations: [
    trigger('carousel', [
      state('left', style({ transform: 'translateX(-101%)' })),
      state('center', style({ transform: 'translateX(0)' })),
      state('right', style({ transform: 'translateX(101%)' })),
      state('hidden', style({ display: 'none' })),
      transition(
        'right => center',
        animate(timings, keyframes([style({ transform: 'translateX(101%)' }), style({ transform: 'translateX(0)' })])),
      ),
      transition(
        'left => center',
        animate(timings, keyframes([style({ transform: 'translateX(-101%)' }), style({ transform: 'translateX(0)' })])),
      ),
      transition('center => right', animate(timings, style({ transform: 'translateX(101%)' }))),
      transition('center => left', animate(timings, style({ transform: 'translateX(-101%)' }))),
      transition(
        'hidden => right',
        animate(
          timings,
          keyframes([style({ transform: 'translateX(200%)' }), style({ transform: 'translateX(101%)' })]),
        ),
      ),
      transition(
        'hidden => left',
        animate(
          timings,
          keyframes([style({ transform: 'translateX(-200%)' }), style({ transform: 'translateX(-101%)' })]),
        ),
      ),
      transition('left => hidden', animate(timings, style({ transform: 'translateX(-200%)' }))),
      transition('right => hidden', animate(timings, style({ transform: 'translateX(200%)' }))),
    ]),
  ],
})
export class CarouselComponent extends SectionItemBase implements OnInit {
  constructor(private softwareService: SoftwareService) {
    super();
  }
  click$ = new Subject<string>();
  carousels: SectionCarousel[];

  state: { [key: number]: string } = {
    0: 'left',
    1: 'center',
    2: 'right',
  };
  running$: Observable<void>;
  ngOnInit() {
    this.init().finally(() => this.loaded.emit(true));
    this.running$ = this.click$.pipe(
      throttleTime(500),
      startWith(''),
      switchMap(s => {
        const v = { left: -1, right: 1 };
        const i = v[s] || 0;
        return timer(3000, 3000).pipe(
          map(() => 1),
          startWith(i),
        );
      }),
      map(i => this.move(i)),
    );
  }
  // filter soft
  async init() {
    this.carousels = (this.section.items as SectionCarousel[]) || [];
    if (this.carousels.length === 0) {
      return;
    }
    this.carousels = this.carousels.filter(c => c.show);
    const names = this.carousels.filter(c => c.type === CarouselType.App).map(c => c.link);
    const softs = await this.softwareService.list({ names });
    this.carousels = this.carousels.filter(c => {
      if (c.type) {
        return true;
      }
      return softs.some(soft => soft.name === c.link);
    });
    while (this.carousels.length < 5) {
      this.carousels = [...this.carousels, ...this.carousels];
    }
  }
  // move image (-1,0,1)
  move(i: number) {
    const current = Object.entries(this.state).find(([index, s]) => s === 'center')[0];
    i += Number(current);
    if (i >= this.carousels.length) {
      i = 0;
    }
    if (i < 0) {
      i = this.carousels.length - 1;
    }
    this.state = {};
    this.state[(i === 0 ? this.carousels.length : i) - 1] = 'left';
    this.state[i] = 'center';
    this.state[i === this.carousels.length - 1 ? 0 : i + 1] = 'right';
  }
}
