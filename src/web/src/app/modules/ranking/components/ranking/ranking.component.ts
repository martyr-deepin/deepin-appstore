import { Component, OnInit } from '@angular/core';
import { fromEvent, merge, Subject, BehaviorSubject } from 'rxjs';

import { AppService } from 'app/services/app.service';
import { ActivatedRoute } from '@angular/router';
import {
  startWith,
  switchMap,
  pairwise,
  filter,
  map,
  tap,
  first,
  scan,
  skip,
  mergeMap,
} from 'rxjs/operators';
import { RankingService, ResourceList, App } from '../../ranking.service';

@Component({
  selector: 'dstore-ranking',
  templateUrl: './ranking.component.html',
  styleUrls: ['./ranking.component.scss'],
})
export class RankingComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private rankingService: RankingService,
    private appService: AppService,
  ) {}
  readonly limit = 100;

  list = new BehaviorSubject(this.route.snapshot.data.data);
  apps$ = this.route.data.pipe(
    switchMap(data => {
      this.list = new BehaviorSubject(data.data);
      return this.list.pipe(
        mergeMap(list =>
          this.appService.getApps(list.map(app => app.name)).pipe(first()),
        ),
        scan((acc, value) => [...acc, ...value], []),
      );
    }),
  );

  ngOnInit() {}

  async load() {
    const last = await this.list.pipe(first()).toPromise();
    if (last.end >= this.limit) {
      return;
    }
    const list = await this.rankingService
      .list({
        order: this.route.snapshot.queryParamMap.get('order') as any,
        offset: last.end,
        limit: 20,
      })
      .toPromise();
    this.list.next(list);
  }
}
