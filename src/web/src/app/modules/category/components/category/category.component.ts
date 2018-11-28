import { Component, OnInit, AfterViewChecked } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable, of, forkJoin, Subject } from 'rxjs';
import { flatMap, tap, map, switchMap } from 'rxjs/operators';

import { find, defaults } from 'lodash';

import { AppService, App } from 'app/services/app.service';
import { CategoryService, Category } from 'app/services/category.service';

@Component({
  selector: 'dstore-category',
  templateUrl: './category.component.html',
  styleUrls: ['./category.component.scss'],
})
export class CategoryComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private categoryService: CategoryService,
    private appService: AppService,
  ) {}
  title: string;
  loaded = false;
  apps$ = this.route.paramMap.pipe(
    tap(() => {
      this.loaded = false;
      setTimeout(() => {
        this.title = document.querySelector('.navItem.active').textContent;
      }, 0);
    }),
    map(param => param.get('id')),
    switchMap(id => {
      return this.categoryService.list().pipe(
        switchMap(cs => {
          const c = find(cs, { id });
          if (c) {
            return this.appService.getApps(c.apps);
          } else {
            return this.appService.getAppListByCategory(id);
          }
        }),
      );
    }),
    tap(() => {
      this.loaded = true;
    }),
  );
  ngOnInit() {}
}
