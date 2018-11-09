import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { IndexComponent } from 'app/components/index/index.component';
import { CategoryComponent } from 'app/components/category/category.component';
import { RankingComponent } from 'app/components/ranking/ranking.component';
import { DownloadComponent } from 'app/components/app-manage/download/download.component';
import { UninstallComponent } from 'app/components/app-manage/uninstall/uninstall.component';
import { SearchComponent } from 'app/components/search/search.component';
import { TopicComponent } from 'app/components/topic/topic.component';
import { MoreComponent } from 'app/components/more/more.component';

const routes: Routes = [
  {
    path: '',
    redirectTo: 'index',
    pathMatch: 'full',
  },
  // 保持导航关联
  {
    path: 'app/:appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },
  {
    path: 'category/:id/:appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },
  {
    path: 'uninstall/:appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },
  {
    path: 'download/:appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },

  { path: 'index', component: IndexComponent },
  { path: 'index/apps', component: MoreComponent },
  { path: 'index/apps/:appName', redirectTo: 'app/:appName' },
  { path: 'index/:appName', redirectTo: 'app/:appName' },
  {
    path: 'category/:id',
    component: CategoryComponent,
  },
  {
    path: 'ranking',
    component: RankingComponent,
  },
  {
    path: 'ranking/:appName',
    redirectTo: 'app/:appName',
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
    path: 'search',
    component: SearchComponent,
  },
  {
    path: 'search/:appName',
    redirectTo: 'app/:appName',
  },
  {
    path: 'topic/:section/:topic',
    component: TopicComponent,
  },
  {
    path: 'topic/:section/:topic/:appName',
    redirectTo: 'app/:appName',
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
