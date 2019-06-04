import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { combineLatest, BehaviorSubject, timer } from 'rxjs';
import { switchMap, retryWhen, scan, first, map } from 'rxjs/operators';
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

  type$ = this.route.paramMap.pipe(map(param => param.get('type')));
  // loading offset
  offset$ = new BehaviorSubject(0);
  result$ = combineLatest(this.route.paramMap, this.route.queryParamMap).pipe(
    switchMap(([param, query]) => {
      const [routeName, routeValue] = [param.get('name'), param.get('value')];
      const order = (query.get('order') as any) || 'download';
      this.offset$ = new BehaviorSubject(0);
      return this.offset$.pipe(
        switchMap(offset =>
          this.rankingService.list({ order, offset, [routeName]: routeValue }),
        ),
        retryWhen(errors => errors.pipe(switchMap(() => timer(1000)))),
        scan((acc, value) => [...acc, ...value], []),
      );
    }),
  );

  ngOnInit() {
    console.log(this.route.snapshot.paramMap);
  }

  load() {
    this.offset$.pipe(first()).subscribe(offset => {
      this.offset$.next(offset + 20);
    });
  }
}
