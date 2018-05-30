import { Component, OnInit } from '@angular/core';
import { DomSanitizer, SafeUrl, SafeStyle } from '@angular/platform-browser';
import { Observable, timer } from 'rxjs';
import { tap, flatMap, map } from 'rxjs/operators';

import { memoize } from 'lodash';

import { CategoryService, makeDefaultCategory } from '../../services/category.service';
import { Category } from '../../services/category.service';
import { BaseService } from '../../dstore/services/base.service';
import { StoreService } from '../../dstore-client.module/services/store.service';

@Component({
  selector: 'app-side-nav',
  templateUrl: './side-nav.component.html',
  styleUrls: ['./side-nav.component.scss'],
})
export class SideNavComponent implements OnInit {
  constructor(
    private categoryService: CategoryService,
    private sanitizer: DomSanitizer,
    private storeService: StoreService,
  ) {}
  native = BaseService.isNative;
  default = makeDefaultCategory();
  cs$: Observable<Category[]>;
  // download count
  dc$: Observable<number>;

  getStyle = memoize((icon: string[]) => {
    return this.sanitizer.bypassSecurityTrustStyle(
      `content: url(${icon[0]});
       --active: url(${icon[1]})`,
    );
  });
  getStyleByID = memoize((id: string) => {
    return this.sanitizer.bypassSecurityTrustStyle(
      `content: url("/assets/icons/${id}.svg");
       --active: url("/assets/icons/${id}_active.svg")`,
    );
  });
  ngOnInit() {
    this.cs$ = this.categoryService.list();
    this.dc$ = timer(0, 1000).pipe(
      flatMap(() => this.storeService.getJobList()),
      map(jobs => jobs.filter(job => job.includes('install')).length),
    );
  }
}
