import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { UninstallComponent } from './components/uninstall/uninstall.component';

const routes: Routes = [
  {
    path: '',
    component: UninstallComponent,
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
export class UninstallRoutingModule {}
