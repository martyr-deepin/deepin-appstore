import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { TagAppsComponent } from './components/tag-apps/tag-apps.component';

const routes: Routes = [
  {
    path: '',
    component: TagAppsComponent,
  },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class TagsRoutingModule {}
