import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { IndexComponent } from '../components/index/index.component';
import { CategoryComponent } from '../components/category/category.component';
import { RankingComponent } from '../components/ranking/ranking.component';
import { DownloadComponent } from '../components/app-manage/download/download.component';
import { UninstallComponent } from '../components/app-manage/uninstall/uninstall.component';
import { AppDetailComponent } from '../components/app-detail/app-detail.component';
import { SearchComponent } from '../components/search/search.component';
import { TopicComponent } from '../components/topic/topic.component';
import { MoreComponent } from '../components/more/more.component';

const routes: Routes = [
  {
    path: '',
    redirectTo: 'index',
    pathMatch: 'full',
  },
  { path: 'index', component: IndexComponent },
  { path: 'index/apps', component: MoreComponent },
  { path: 'index/apps/:appName', component: AppDetailComponent },
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
    path: 'uninstall/:appName',
    component: AppDetailComponent,
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
  imports: [
    RouterModule.forRoot(routes, {
      enableTracing: false,
    }),
  ],
  exports: [RouterModule],
})
export class RoutingModule {}
