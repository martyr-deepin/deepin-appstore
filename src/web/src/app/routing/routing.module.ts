import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { UninstallComponent } from '../uninstall/uninstall.component';
import { DownloadComponent } from '../download/download.component';
import { UpdateComponent } from '../update/update.component';

const routes: Routes = [
  {
    path: 'download',
    component: DownloadComponent
  },
  {
    path: 'update',
    component: UpdateComponent
  },
  {
    path: 'uninstall',
    component: UninstallComponent
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes, {enableTracing: false})],
  exports: [RouterModule]
})
export class RoutingModule {
}
