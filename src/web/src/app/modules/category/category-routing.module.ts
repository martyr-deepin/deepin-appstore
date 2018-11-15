import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { CategoryComponent } from './components/category/category.component';

const routes: Routes = [
  {
    path: ':id',
    component: CategoryComponent,
  },
  {
    path: ':id/:appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class CategoryRoutingModule {}
