import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs';
import { map, flatMap, switchMap, tap } from 'rxjs/operators';
import { truncate } from 'lodash';

import { App, AppService } from 'app/services/app.service';
import { RecommendService } from 'app/services/recommend.service';

@Component({
  selector: 'app-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.scss'],
})
export class SearchComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private appService: AppService,
    private recommendService: RecommendService,
  ) {}

  keyword$: Observable<string>;
  title$: Observable<string>;
  apps$: Observable<App[]>;
  loaded = false;

  ngOnInit() {
    this.title$ = this.route.queryParamMap.pipe(
      map(param => `"${truncate(param.get('keyword'), { length: 16 })}"`),
    );

    this.apps$ = this.route.queryParamMap.pipe(
      switchMap(param => {
        this.loaded = false;
        const appNameList = param.getAll('apps');
        return this.appService.getApps(appNameList);
      }),
      tap(() => {
        this.loaded = true;
      }),
    );
  }

  recommend() {
    this.recommendService.openRecommend();
  }
}
