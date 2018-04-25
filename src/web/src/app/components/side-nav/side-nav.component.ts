import { Component, OnInit } from '@angular/core';

import { CategoryService } from '../../services/category.service';
import { Category } from '../../services/category.service';

@Component({
  selector: 'app-side-nav',
  templateUrl: './side-nav.component.html',
  styleUrls: ['./side-nav.component.scss'],
})
export class SideNavComponent implements OnInit {
  constructor(private categoryService: CategoryService) {}

  cs: Category[];
  ngOnInit() {
    this.categoryService.list().subscribe(cs => (this.cs = cs));
  }
}
