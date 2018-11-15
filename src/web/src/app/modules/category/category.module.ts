import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { CategoryRoutingModule } from './category-routing.module';
import { CategoryComponent } from './components/category/category.component';
import { ShareModule } from '../share/share.module';

@NgModule({
  declarations: [CategoryComponent],
  imports: [CommonModule, CategoryRoutingModule, ShareModule],
})
export class CategoryModule {}
