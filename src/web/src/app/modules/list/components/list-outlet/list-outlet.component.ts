import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { combineLatest, BehaviorSubject, timer } from 'rxjs';
import {
  switchMap,
  retryWhen,
  scan,
  first,
  map,
  share,
  tap,
} from 'rxjs/operators';
import { RankingService } from 'app/modules/ranking/ranking.service';

@Component({
  selector: 'dstore-list-outlet',
  templateUrl: './list-outlet.component.html',
  styleUrls: ['./list-outlet.component.scss'],
})
export class ListOutletComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private rankingService: RankingService,
  ) {}
  title = '';
  name$ = this.route.paramMap.pipe(map(param => param.get('name')));
  // loading offset
  offset$ = new BehaviorSubject(0);
  result$ = combineLatest(this.route.paramMap, this.route.queryParamMap).pipe(
    switchMap(([param, query]) => {
      setTimeout(() => {
        const el = document.querySelector<HTMLDivElement>('.navItem.active');
        if (el) {
          this.title = el.innerText.trim();
        }
      });
      const [routeName, routeValue] = [param.get('name'), param.get('value')];
      const order = (query.get('order') as any) || 'download';
      this.offset$ = new BehaviorSubject(0);
      this.offset$.subscribe(offset => console.log('offset', offset));
      return this.offset$.pipe(
        switchMap(offset =>
          this.rankingService.list({ order, offset, [routeName]: routeValue }),
        ),
        retryWhen(errors =>
          errors.pipe(
            tap(console.error),
            switchMap(err => timer(1000)),
          ),
        ),
        scan((acc, value) => [...acc, ...value], []),
      );
    }),
    share(),
  );

  ngOnInit() {}

  load() {
    this.offset$.pipe(first()).subscribe(offset => {
      this.offset$.next(offset + 20);
    });
  }
}
