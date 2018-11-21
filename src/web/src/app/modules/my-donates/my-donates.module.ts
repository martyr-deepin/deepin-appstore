import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { MyDonatesRoutingModule } from './my-donates-routing.module';
import { DonatesComponent } from './donates/donates.component';
import { ShareModule } from '../share/share.module';

@NgModule({
  declarations: [DonatesComponent],
  imports: [CommonModule, MyDonatesRoutingModule, ShareModule],
})
export class MyDonatesModule {}
