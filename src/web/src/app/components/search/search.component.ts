import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs/Observable';

import { App, AppService } from '../../services/app.service';
import { SortOrder } from '../app-title/app-title.component';

@Component({
  selector: 'app-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.scss'],
})
export class SearchComponent implements OnInit {
  constructor(private route: ActivatedRoute, private appService: AppService) {}

  sortBy = SortOrder.Downloads;
  keyword: string;
  apps$: Observable<App[]>;
  ngOnInit() {
    this.apps$ = this.route.paramMap.mergeMap(param => {
      this.keyword = param.get('keyword');
      // angular bug https://github.com/angular/angular/issues/19179
      const appNameList = param
        .getAll('apps')
        .map(apps => apps.split(','))
        .reduce((a, b) => [...a, ...b]);
      return this.appService
        .list()
        .map(apps => apps.filter(app => appNameList.includes(app.name)));
    });
  }
}
