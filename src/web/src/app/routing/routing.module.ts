import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { IndexComponent } from '../components/index/index.component';
import { CategoryComponent } from '../components/category/category.component';
import { RankingComponent } from '../components/ranking/ranking.component';
import { DownloadComponent } from '../components/download-manage/download-manage.component';
import { UninstallComponent } from '../components/uninstall/uninstall.component';
import { AppDetailComponent } from '../components/app-detail/app-detail.component';
import { SearchComponent } from '../components/search/search.component';
import { TopicComponent } from '../components/topic/topic.component';

const routes: Routes = [
  {
    path: '',
    redirectTo: 'index',
    pathMatch: 'full',
  },
  { path: 'index', component: IndexComponent },
  { path: 'index/:appName', component: AppDetailComponent },
  {
    path: 'category/:id',
    component: CategoryComponent,
  },
  {
    path: 'category/:id/:appName',
    component: AppDetailComponent,
  },
  {
    path: 'ranking',
    component: RankingComponent,
  },
  {
    path: 'ranking/:appName',
    component: AppDetailComponent,
  },
  {
    path: 'uninstall',
    component: UninstallComponent,
  },
  {
    path: 'download',
    component: DownloadComponent,
  },
  {
    path: 'download/:appName',
    component: AppDetailComponent,
  },
  {
    path: 'search',
    component: SearchComponent,
  },
  {
    path: 'search/:appName',
    component: AppDetailComponent,
  },
  {
    path: 'topic/:section/:topic',
    component: TopicComponent,
  },
  {
    path: 'topic/:section/:topic/:appName',
    component: AppDetailComponent,
  },
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { enableTracing: false })],
  exports: [RouterModule],
})
export class RoutingModule {}
