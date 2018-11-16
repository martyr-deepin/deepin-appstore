import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

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
    path: 'index',
    loadChildren: 'app/modules/index/index.module#IndexModule',
  },
  {
    path: 'category',
    loadChildren: 'app/modules/category/category.module#CategoryModule',
  },
  {
    path: 'ranking',
    loadChildren: 'app/modules/ranking/ranking.module#RankingModule',
  },
  {
    path: 'download',
    loadChildren: 'app/modules/download/download.module#DownloadModule',
  },
  {
    path: 'uninstall',
    loadChildren: 'app/modules/uninstall/uninstall.module#UninstallModule',
  },
  {
    path: 'my/app',
    loadChildren: 'app/modules/my-app/my-app.module#MyAppModule',
  },
  {
    path: 'search',
    loadChildren: 'app/modules/search/search.module#SearchModule',
  },
  {
    path: 'tag/:tag',
    loadChildren: 'app/modules/tags/tags.module#TagsModule',
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
