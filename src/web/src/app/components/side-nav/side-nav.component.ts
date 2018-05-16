import { Component, OnInit } from '@angular/core';
import { DomSanitizer, SafeUrl, SafeStyle } from '@angular/platform-browser';
import { Observable } from 'rxjs';
import { tap } from 'rxjs/operators';

import { memoize } from 'lodash';

import { CategoryService, makeDefaultCategory } from '../../services/category.service';
import { Category } from '../../services/category.service';
import { BaseService } from '../../dstore/services/base.service';

@Component({
  selector: 'app-side-nav',
  templateUrl: './side-nav.component.html',
  styleUrls: ['./side-nav.component.scss'],
})
export class SideNavComponent implements OnInit {
  constructor(private categoryService: CategoryService, private sanitizer: DomSanitizer) {}
  native = BaseService.isNative;
  default = makeDefaultCategory();
  cs$: Observable<Category[]>;

  getStyle = memoize((icon: string[]) => {
    return this.sanitizer.bypassSecurityTrustStyle(
      `content: url(${icon[0]});
       --active: url(${icon[1]})`,
    );
  });
  getStyleByID = memoize((id: string) => {
    return this.sanitizer.bypassSecurityTrustStyle(
      `content: url("/assets/category/${id}.svg");
       --active: url("/assets/category/${id}_active.svg")`,
    );
  });
  ngOnInit() {
    this.cs$ = this.categoryService.list();
  }
}
