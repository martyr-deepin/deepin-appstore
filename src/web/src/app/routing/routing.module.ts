import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { CategoryComponent } from 'app/components/category/category.component';
import { RankingComponent } from 'app/components/ranking/ranking.component';
import { DownloadComponent } from 'app/components/app-manage/download/download.component';
import { UninstallComponent } from 'app/components/app-manage/uninstall/uninstall.component';

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

  { path: 'index', loadChildren: 'app/modules/index/index.module#IndexModule' },
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
