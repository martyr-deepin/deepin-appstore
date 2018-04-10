import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';


import { AppComponent } from './app.component';
import { UpdateComponent } from './update/update.component';
import { DownloadComponent } from './download/download.component';
import { UninstallComponent } from './uninstall/uninstall.component';
import { RoutingModule } from './routing/routing.module';
import { NavBarComponent } from './nav-bar/nav-bar.component';


@NgModule({
  declarations: [
    AppComponent,
    DownloadComponent,
    UninstallComponent,
    UpdateComponent,
    NavBarComponent
  ],
  imports: [
    BrowserModule,
    RoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule {
}
