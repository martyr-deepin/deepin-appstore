import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs';
import { flatMap, tap, map, defaultIfEmpty } from 'rxjs/operators';

import { find } from 'lodash';

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
          .pipe(map(cs => find(cs, { id })), map(category => category || { title: id }));
      }),
      tap(category => {
        console.log('category', category);
        this.apps$ = this.appService
          .list()
          .pipe(
            map(
              apps =>
                category.apps
                  ? apps.filter(app => category.apps.includes(app.name))
                  : apps.filter(app => app.category === category.id),
            ),
          );
      }),
      map(category => category.title),
    );
  }
}
