import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { MyAppRoutingModule } from './my-app-routing.module';
import { LocalAppComponent } from './components/local-app/local-app.component';
import { RemoteAppComponent } from './components/remote-app/remote-app.component';
import { ShareModule } from '../share/share.module';
import { RemoteInstallComponent } from './components/remote-install/remote-install.component';
import { SwitchButtonComponent } from './components/switch-button/switch-button.component';
import { BatchInstallComponent } from './components/batch-install/batch-install.component';
import { CheckboxButtonComponent } from './components/checkbox-button/checkbox-button.component';

@NgModule({
  declarations: [LocalAppComponent, RemoteAppComponent, RemoteInstallComponent, SwitchButtonComponent, BatchInstallComponent, CheckboxButtonComponent],
  imports: [CommonModule, MyAppRoutingModule, ShareModule],
})
export class MyAppModule {}
