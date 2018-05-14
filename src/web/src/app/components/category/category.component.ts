import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable, of, forkJoin } from 'rxjs';
import { flatMap, tap, map, defaultIfEmpty, share } from 'rxjs/operators';

import { find, defaults } from 'lodash';

import { AppService } from '../../services/app.service';
import { App } from '../../dstore/services/app';
import { CategoryService, Category } from '../../services/category.service';
import { SortOrder } from '../app-title/app-title.component';

@Component({
  selector: 'app-category',
  templateUrl: './category.component.html',
  styleUrls: ['./category.component.scss'],
})
export class CategoryComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private categoryService: CategoryService,
    private appService: AppService,
  ) {}

  title$: Observable<string>;
  apps$: Observable<App[]>;

  ngOnInit() {
    this.title$ = this.route.paramMap.pipe(
      flatMap(param => {
        const id = param.get('id');
        return this.categoryService
          .list()
          .pipe(map(cs => find(cs, { id })), map(category => category));
      }),
      tap(category => {
        console.log('category', category);
        // 切换分类重赋值apps，以展现加载动画
        if (category.apps) {
          this.apps$ = this.appService.getApps(category.apps);
        } else {
          this.apps$ = this.appService.getAppListByCategory(category.id);
        }
      }),
      map(category => category.title),
    );
  }
}
