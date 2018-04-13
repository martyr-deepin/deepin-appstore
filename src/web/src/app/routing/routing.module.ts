import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { IndexComponent } from '../components/index/index.component';
import { CategoryComponent } from '../components/category/category.component';
import { RankingComponent } from '../components/ranking/ranking.component';
import { DownloadComponent } from '../components/download/download.component';
import { UpdateComponent } from '../components/update/update.component';
import { UninstallComponent } from '../components/uninstall/uninstall.component';
import { AppDetailComponent } from '../components/app-detail/app-detail.component';

const routes: Routes = [
  { path: 'index', component: IndexComponent },
  {
    path: 'category/:id',
    component: CategoryComponent
  },
  {
    path: 'category/:id/:appName',
    component: AppDetailComponent
  },
  {
    path: 'ranking',
    component: RankingComponent
  },
  {
    path: 'ranking/:appName',
    component: AppDetailComponent
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
