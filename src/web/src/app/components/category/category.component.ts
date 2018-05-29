import { Component, OnInit, AfterViewChecked } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable, of, forkJoin, Subject } from 'rxjs';
import { flatMap, tap, map, defaultIfEmpty, share, distinctUntilChanged } from 'rxjs/operators';

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
  title: string;
  apps$: Observable<App[]>;

  list$: Observable<{ title: string; apps$: Observable<App[]> }>;

  ngOnInit() {
    this.list$ = this.route.paramMap.pipe(
      flatMap(param => {
        setTimeout(() => {
          this.title = document.querySelector('.navItem.active').textContent;
        }, 0);

        const id = param.get('id');
        return this.categoryService.list().pipe(
          map(cs => {
            const c = find(cs, { id });
            if (c) {
              return { title: c.title, apps$: this.appService.getApps(c.apps) };
            } else {
              return { title: id, apps$: this.appService.getAppListByCategory(id) };
            }
          }),
        );
      }),
    );
  }
}
