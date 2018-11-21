import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { DonatesComponent } from './donates/donates.component';

const routes: Routes = [
  {
    path: '',
    component: DonatesComponent,
  },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class MyDonatesRoutingModule {}
