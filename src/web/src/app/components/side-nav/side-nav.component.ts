import { Component, OnInit } from '@angular/core';
import { DomSanitizer, SafeUrl, SafeStyle } from '@angular/platform-browser';
import { Observable } from 'rxjs';
import { memoize } from 'lodash';

import { CategoryService } from '../../services/category.service';
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
  cs$: Observable<Category[]>;

  getStyle = memoize((id: string) => {
    return this.sanitizer.bypassSecurityTrustStyle(
      `--src: url("/assets/category/${id}.svg");
       --active: url("/assets/category/${id}_active.svg")`,
    );
  });
  ngOnInit() {
    this.cs$ = this.categoryService.list();
  }
}
