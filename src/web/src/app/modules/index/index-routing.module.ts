import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { IndexComponent } from './index.component';
import { MoreComponent } from './components/more/more.component';
import { TopicComponent } from './components/topic/topic.component';
import { TopicDetailComponent } from './components/topic-detail/topic-detail.component';

const routes: Routes = [
  {
    path: '',
    component: IndexComponent,
  },
  {
    path: 'more/:key',
    component: MoreComponent,
  },
  {
    path: 'topic/:key',
    component: TopicDetailComponent,
  },
  {
    path: 'topic/:key/:appName',
    redirectTo: 'app/:appName',
  },
  {
    path: 'app/:appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class IndexRoutingModule {}
