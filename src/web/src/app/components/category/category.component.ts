import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs';

import * as _ from 'lodash';

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

  categoryObs: Observable<Category>;
  appsObs: Observable<App[]>;

  sortBy = SortOrder.Downloads;

  ngOnInit() {
    this.categoryObs = this.route.paramMap
      .mergeMap(param => {
        return this.categoryService.list().map(cs => _.find(cs, c => c.id === param.get('id')));
      })
      .filter(c => c !== undefined);
    this.appsObs = this.categoryObs.mergeMap(c => {
      return this.appService.list().map(apps => {
        if (c.apps && c.apps.length > 0) {
          return apps.filter(app => c.apps.includes(app.name));
        } else {
          return apps.filter(app => app.category === c.id);
        }
      });
    });
  }
}
