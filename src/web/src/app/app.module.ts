import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { NgModule } from '@angular/core';
import { HTTP_INTERCEPTORS, HttpClientModule } from '@angular/common/http';

import { registerLocaleData } from '@angular/common';
import localeZH from '@angular/common/locales/zh-Hans';
registerLocaleData(localeZH, 'zh-Hans');

import { RoutingModule } from './routing/routing.module';
import { DstoreModule } from './dstore/dstore.module';
import { ClientModule } from 'app/modules/client/client.module';
import { ShareModule } from 'app/modules/share/share.module';

import { AuthInterceptor } from './services/auth-interceptor';

import { AppService } from './services/app.service';
import { AuthService } from './services/auth.service';
import { AuthGuardService } from './services/auth-guard.service';
import { SearchService } from './services/search.service';
import { LoginService } from './services/login.service';
import { RecommendService } from './services/recommend.service';

import { AppComponent } from './app.component';
import { SideNavComponent } from './components/side-nav/side-nav.component';
import { LoginComponent } from './components/login/login.component';
import { RecommendComponent } from './components/recommend/recommend.component';
import { NotifyComponent } from './components/notify/notify.component';

@NgModule({
  declarations: [
    AppComponent,
    SideNavComponent,
    LoginComponent,
    RecommendComponent,
    NotifyComponent,
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    RoutingModule,
    DstoreModule,
    ClientModule,
    ShareModule,
  ],
  providers: [{ provide: HTTP_INTERCEPTORS, useClass: AuthInterceptor, multi: true }],
  bootstrap: [AppComponent],
})
export class AppModule {}
