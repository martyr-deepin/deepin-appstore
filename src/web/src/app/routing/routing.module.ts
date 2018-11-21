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
    redirectTo: 'my/apps',
  },
  {
    path: 'my/apps',
    loadChildren: 'app/modules/my-apps/my-apps.module#MyAppsModule',
  },
  {
    path: 'my/comments',
    loadChildren: 'app/modules/my-comments/my-comments.module#MyCommentsModule',
  },
  {
    path: 'my/donates',
    loadChildren: 'app/modules/my-donates/my-donates.module#MyDonatesModule',
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
