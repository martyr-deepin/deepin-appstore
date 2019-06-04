import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { ListRoutingModule } from './list-routing.module';
import { ListOutletComponent } from './components/list-outlet/list-outlet.component';
import { ListComponent } from './components/list/list.component';
import { ShareModule } from '../share/share.module';
import { PackagePipe } from './pipes/package.pipe';

@NgModule({
  declarations: [ListOutletComponent, ListComponent, PackagePipe],
  imports: [CommonModule, ListRoutingModule, ShareModule],
})
export class ListModule {}
