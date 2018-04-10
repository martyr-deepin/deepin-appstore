import { BrowserModule } from '@angular/platform-browser';
import { LOCALE_ID, NgModule } from '@angular/core';


import { AppComponent } from './app.component';
import { UpdateComponent } from './update/update.component';
import { DownloadComponent } from './download/download.component';
import { UninstallComponent } from './uninstall/uninstall.component';
import { RoutingModule } from './routing/routing.module';
import { NavBarComponent } from './nav-bar/nav-bar.component';
import { Locale } from './utils/locale';
import { AppDetailComponent } from './app-detail/app-detail.component';
import { CategoryComponent } from './category/category.component';


@NgModule({
  declarations: [
    AppComponent,
    AppDetailComponent,
    CategoryComponent,
    DownloadComponent,
    UninstallComponent,
    UpdateComponent,
    NavBarComponent
  ],
  imports: [
    BrowserModule,
    RoutingModule
  ],
  providers: [{
    provide: LOCALE_ID,
    useValue: Locale.getPcp47Locale()
  }],
  bootstrap: [AppComponent]
})
export class AppModule {
}
