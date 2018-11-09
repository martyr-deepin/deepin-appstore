import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { IndexComponent } from 'app/components/index/index.component';
import { CategoryComponent } from 'app/components/category/category.component';
import { RankingComponent } from 'app/components/ranking/ranking.component';
import { DownloadComponent } from 'app/components/app-manage/download/download.component';
import { UninstallComponent } from 'app/components/app-manage/uninstall/uninstall.component';
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
    path: 'index/apps/:appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },
  {
    path: 'ranking/:appName',
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
  {
    path: 'category/:id',
    component: CategoryComponent,
  },
  {
    path: 'ranking',
    component: RankingComponent,
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
    loadChildren: 'app/modules/search/search.module#SearchModule',
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
