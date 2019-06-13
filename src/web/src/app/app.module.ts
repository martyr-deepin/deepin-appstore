import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { NgModule } from '@angular/core';
import { HTTP_INTERCEPTORS, HttpClientModule } from '@angular/common/http';
import { registerLocaleData } from '@angular/common';
import localeZH from '@angular/common/locales/zh-Hans';
registerLocaleData(localeZH, 'zh-Hans');

import { RoutingModule } from './routing/routing.module';
import { ClientModule } from 'app/modules/client/client.module';
import { ShareModule } from 'app/modules/share/share.module';

import { AuthInterceptor } from './services/auth-interceptor';

import { AppComponent } from './app.component';
import { MainComponent } from './components/main/main.component';
import { SideNavComponent } from './components/side-nav/side-nav.component';
import { RecommendComponent } from './components/recommend/recommend.component';
import { NotifyComponent } from './components/notify/notify.component';
import { PrivacyAgreementComponent } from './components/privacy-agreement/privacy-agreement.component';

@NgModule({
  declarations: [
    AppComponent,
    SideNavComponent,
    RecommendComponent,
    NotifyComponent,
    PrivacyAgreementComponent,
    MainComponent,
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    RoutingModule,
    ClientModule,
    ShareModule,
  ],
  providers: [{ provide: HTTP_INTERCEPTORS, useClass: AuthInterceptor, multi: true }],
  bootstrap: [AppComponent],
})
export class AppModule {}
