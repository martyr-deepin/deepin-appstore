import { Component, OnInit, ViewChild, ElementRef, ChangeDetectionStrategy } from '@angular/core';
import { BehaviorSubject, timer } from 'rxjs';

import { ActivatedRoute } from '@angular/router';
import { switchMap, first, scan, retryWhen } from 'rxjs/operators';
import { RankingService } from '../../ranking.service';

@Component({
  selector: 'dstore-ranking',
  templateUrl: './ranking.component.html',
  styleUrls: ['./ranking.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush,
})
export class RankingComponent implements OnInit {
  constructor(private route: ActivatedRoute, private rankingService: RankingService) {}
  readonly maxLimit = 500;
  @ViewChild('footerRef') footer: ElementRef<HTMLElement>;

  offset$: BehaviorSubject<number>;

  // 无限加载
  result$ = this.route.queryParamMap.pipe(
    switchMap(query => {
      const order = (query.get('order') as any) || 'download';
      const data = this.route.snapshot.data.data as ResolveModel[];
      this.offset$ = new BehaviorSubject(data.length);
      return this.offset$.pipe(
        switchMap(offset => this.rankingService.list({ order, offset })),
        retryWhen(errors => errors.pipe(switchMap(() => timer(1000)))),
        scan((acc, value) => [...acc, ...value], data),
      );
    }),
  );

  ngOnInit() {}

  loading() {
    this.offset$.pipe(first()).subscribe(offset => {
      this.offset$.next(offset + 20);
    });
  }
}
