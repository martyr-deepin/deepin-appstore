import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { DownloadComponent } from './components/download/download.component';

const routes: Routes = [
  {
    path: '',
    component: DownloadComponent,
  },
  {
    path: ':appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class DownloadRoutingModule {}
