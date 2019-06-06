import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { IndexComponent } from './index.component';
import { MoreComponent } from './components/more/more.component';
import { TopicComponent } from './components/topic/topic.component';

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
    path: 'app/:appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
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
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class IndexRoutingModule {}
