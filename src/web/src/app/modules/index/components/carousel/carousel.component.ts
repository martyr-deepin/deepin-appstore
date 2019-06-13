import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { trigger, transition, animate, keyframes, style, state } from '@angular/animations';
import { Observable, Subject, timer } from 'rxjs';
import { throttleTime, switchMap, startWith, map } from 'rxjs/operators';
import { get } from 'lodash';

import { SectionItemBase } from '../section-item-base';
import { SectionCarousel, CarouselType, SectionService } from '../../services/section.service';
import { SoftwareService } from 'app/services/software.service';
import { KeyvalueService } from 'app/services/keyvalue.service';

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
          keyframes([
            style({ display: 'block', transform: 'translateX(200%)' }),
            style({ transform: 'translateX(101%)' }),
          ]),
        ),
      ),
      transition(
        'hidden => left',
        animate(
          timings,
          keyframes([
            style({ display: 'block', transform: 'translateX(-200%)' }),
            style({ transform: 'translateX(-101%)' }),
          ]),
        ),
      ),
      transition('left => hidden', animate(timings, style({ transform: 'translateX(-200%)' }))),
      transition('right => hidden', animate(timings, style({ transform: 'translateX(200%)' }))),
    ]),
  ],
})
export class CarouselComponent extends SectionItemBase implements OnInit {
  constructor(
    private router: Router,
    private keyvalue: KeyvalueService,
    private sectionService: SectionService,
    private softwareService: SoftwareService,
  ) {
    super();
  }
  click$ = new Subject<string>();
  carousels: SectionCarousel[];
  current: Ring<SectionCarousel>;
  state: { [key: number]: string } = {
    0: 'left',
    1: 'center',
    2: 'right',
  };
  running$: Observable<void>;
  ngOnInit() {
    this.init().finally(() => {
      this.loaded.emit(true);

      this.current = new Ring(this.carousels);
      setTimeout(() => this.move(0), 0);
      this.running$ = this.click$.pipe(
        throttleTime(500),
        switchMap(s => {
          const v = { left: -1, right: 1 };
          const i = v[s] || 0;
          if (i === 0) {
            const c = this.current.value();
            if (c.type === CarouselType.App) {
              this.router.navigate(['app', c.link]);
            } else {
              console.log(c.link);
              const [, , sindex, tindex] = c.link.split('/').map(Number);
              this.sectionService.list.then(list => {
                const topic = get(list, [sindex, 'items', tindex]);
                if (!topic) {
                  this.router.navigate(['app', Math.random()]);
                  return;
                }
                this.router.navigate(['/index/topic', this.keyvalue.add(topic)]);
              });
            }
          } else {
            this.move(i);
          }
          return timer(3000, 3000).pipe(map(() => 1));
        }),
        map(i => {
          this.move(i);
        }),
      );
    });
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
      if (c.type === CarouselType.Topic) {
        return true;
      }
      return softs.some(soft => soft.name === c.link);
    });
    while (this.carousels.length < 5) {
      this.carousels = [...this.carousels, ...this.carousels];
    }
  }
  async goto(index: number) {
    const left = [];
    for (let c = this.current; c.index !== index; c = c.prev()) {
      left.push(-1);
    }
    const right = [];
    for (let c = this.current; c.index !== index; c = c.next()) {
      right.push(1);
    }
    const sorttest = [right, left].sort((a, b) => a.length - b.length)[0];
    for (const i of sorttest) {
      this.move(i);
      this.click$.next('');
      await new Promise(resolve => setTimeout(resolve, 200));
    }
  }
  // move image (-1,1)
  move(i: number) {
    switch (i) {
      case -1:
        this.current = this.current.prev();
        break;
      case 1:
        this.current = this.current.next();
        break;
    }
    this.state = {
      [this.current.prev().index]: 'left',
      [this.current.index]: 'center',
      [this.current.next().index]: 'right',
    };
  }
}

class Ring<T> {
  constructor(private data: Array<T>, public readonly index = 0) {}
  prev() {
    const p = this.index <= 0 ? this.data.length - 1 : this.index - 1;
    return new Ring(this.data, p);
  }
  next() {
    const n = this.index >= this.data.length - 1 ? 0 : this.index + 1;
    return new Ring(this.data, n);
  }
  value() {
    return this.data[this.index];
  }
}
