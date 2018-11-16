import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { MyAppRoutingModule } from './my-app-routing.module';
import { LocalAppComponent } from './components/local-app/local-app.component';
import { RemoteAppComponent } from './components/remote-app/remote-app.component';
import { ShareModule } from '../share/share.module';

@NgModule({
  declarations: [LocalAppComponent, RemoteAppComponent],
  imports: [CommonModule, MyAppRoutingModule, ShareModule],
})
export class MyAppModule {}
