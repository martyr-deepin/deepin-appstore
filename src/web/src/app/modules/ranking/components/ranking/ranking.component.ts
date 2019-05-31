import {
  Component,
  OnInit,
  ViewChild,
  ElementRef,
  ChangeDetectionStrategy,
  AfterViewChecked,
  AfterViewInit,
} from '@angular/core';
import { BehaviorSubject } from 'rxjs';

import { ActivatedRoute } from '@angular/router';
import { switchMap, first, scan, mergeMap, tap } from 'rxjs/operators';
import { RankingService, ResolveModel } from '../../ranking.service';

@Component({
  selector: 'dstore-ranking',
  templateUrl: './ranking.component.html',
  styleUrls: ['./ranking.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush,
})
export class RankingComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private rankingService: RankingService,
  ) {}
  readonly maxLimit = 100;
  @ViewChild('footerRef') footer: ElementRef<HTMLElement>;

  offset$: BehaviorSubject<number>;

  // 无限加载
  result$ = this.route.queryParamMap.pipe(
    switchMap(query => {
      const order = (query.get('order') as any) || 'download';
      const data = this.route.snapshot.data.data as ResolveModel[];
      this.offset$ = new BehaviorSubject(data.length);
      return this.offset$.pipe(
        mergeMap(offset => this.rankingService.list({ order, offset })),
        scan((acc, value) => [...acc, ...value], data),
        tap(() => this.intersection.observe(this.footer.nativeElement)),
      );
    }),
  );

  // 监听是否到达底部
  intersection = new IntersectionObserver(
    ([e]: IntersectionObserverEntry[]) => {
      if (e.isIntersecting) {
        this.offset$.pipe(first()).subscribe(offset => {
          if (offset >= this.maxLimit) {
            return;
          }
          this.offset$.next(offset + this.rankingService.limit);
        });
      }
    },
  );

  ngOnInit() {}
}
