import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { LocalAppComponent } from './components/local-app/local-app.component';
import { RemoteAppComponent } from './components/remote-app/remote-app.component';

const routes: Routes = [
  {
    path: '',
    redirectTo: 'local',
  },
  {
    path: 'local',
    component: LocalAppComponent,
  },
  {
    path: 'remote',
    component: RemoteAppComponent,
  },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class MyAppRoutingModule {}
