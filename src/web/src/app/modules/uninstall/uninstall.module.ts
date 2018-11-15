import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { UninstallRoutingModule } from './uninstall-routing.module';
import { UninstallComponent } from './components/uninstall/uninstall.component';
import { ShareModule } from '../share/share.module';

@NgModule({
  declarations: [UninstallComponent],
  imports: [CommonModule, UninstallRoutingModule, ShareModule],
})
export class UninstallModule {}
