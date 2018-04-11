import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { IndexComponent } from '../components/index/index.component';
import { CategoryComponent } from '../components/category/category.component';
import { RankingComponent } from '../components/ranking/ranking.component';
import { DownloadComponent } from '../components/download/download.component';
import { UpdateComponent } from '../components/update/update.component';
import { UninstallComponent } from '../components/uninstall/uninstall.component';

const routes: Routes = [
  { path: 'index', component: IndexComponent },
  {
    path: 'category/:id',
    component: CategoryComponent
  },
  {
    path: 'ranking',
    component: RankingComponent
  },
  {
    path: 'update',
    component: UpdateComponent
  },
  {
    path: 'uninstall',
    component: UninstallComponent
  },
  {
    path: 'download',
    component: DownloadComponent
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { enableTracing: false })],
  exports: [RouterModule]
})
export class RoutingModule {}
