import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { ListOutletComponent } from './components/list-outlet/list-outlet.component';

const routes: Routes = [{ path: '', component: ListOutletComponent }];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class ListRoutingModule {}
