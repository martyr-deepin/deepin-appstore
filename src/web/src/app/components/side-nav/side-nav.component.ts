import { Component, OnInit } from '@angular/core';

import { CategoryService } from '../../services/category.service';
import { Category } from '../../services/category.service';
import { BaseService } from '../../dstore/services/base.service';
import { Observable } from 'rxjs';

@Component({
  selector: 'app-side-nav',
  templateUrl: './side-nav.component.html',
  styleUrls: ['./side-nav.component.scss'],
})
export class SideNavComponent implements OnInit {
  constructor(private categoryService: CategoryService) {}
  native = BaseService.isNative;
  cs$: Observable<Category[]>;
  ngOnInit() {
    this.cs$ = this.categoryService.list();
  }
}
