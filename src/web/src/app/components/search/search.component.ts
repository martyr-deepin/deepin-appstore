import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs';
import { truncate } from 'lodash';

import { App, AppService } from '../../services/app.service';
import { SortOrder } from '../app-title/app-title.component';
import { RecommendService } from '../../services/recommend.service';

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

  sortBy = SortOrder.Downloads;
  keyword$: Observable<string>;
  title$: Observable<string>;
  apps$: Observable<App[]>;

  ngOnInit() {
    this.keyword$ = this.route.paramMap.map(param => param.get('keyword'));
    this.title$ = this.keyword$.map(keyword => `"${truncate(keyword, { length: 16 })}"`);

    this.apps$ = this.route.paramMap
      .mergeMap(param => {
        // angular 'getAll' bug https://github.com/angular/angular/issues/19179
        const appNameList = param
          .getAll('apps')
          .map(apps => apps.split(','))
          .reduce((a, b) => [...a, ...b]);
        return this.appService
          .list()
          .map(apps => apps.filter(app => appNameList.includes(app.name)));
      })
      .do(console.log);
  }

  recommend() {
    this.recommendService.openRecommend();
  }
}
