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
import { ClientModule } from './dstore-client.module/client.module';
import { ShareModule } from 'app/modules/share/share.module';

import { MyHttpInterceptor } from './services/http-interceptor';

import { AppService } from './services/app.service';
import { CategoryService } from './services/category.service';
import { SectionService } from './services/section.service';
import { AuthService } from './services/auth.service';
import { AuthGuardService } from './services/auth-guard.service';
import { CommentService } from './services/comment.service';
import { BaseService } from './dstore/services/base.service';
import { StoreService } from './dstore-client.module/services/store.service';
import { SearchService } from './services/search.service';
import { LoginService } from './services/login.service';
import { RecommendService } from './services/recommend.service';

import { AppComponent } from './app.component';
import { SideNavComponent } from './components/side-nav/side-nav.component';
import { CategoryComponent } from './components/category/category.component';
import { DownloadComponent } from './components/app-manage/download/download.component';
import { UninstallComponent } from './components//app-manage/uninstall/uninstall.component';
import { RankingComponent } from './components/ranking/ranking.component';
import { LoginComponent } from './components/login/login.component';
import { RecommendComponent } from './components/recommend/recommend.component';
import { NotifyComponent } from './components/notify/notify.component';

import { LodashPipe } from './pipes/lodash.pipe';
import { AppInfoPipe } from './pipes/app-info';
import { StoreJobErrorComponent } from './components/store-job-error/store-job-error.component';

@NgModule({
  declarations: [
    AppComponent,
    CategoryComponent,
    DownloadComponent,
    UninstallComponent,
    SideNavComponent,
    CategoryComponent,
    DownloadComponent,
    UninstallComponent,
    RankingComponent,
    LoginComponent,
    RecommendComponent,
    NotifyComponent,
    LodashPipe,
    AppInfoPipe,
    StoreJobErrorComponent,
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
  providers: [
    AppService,
    CategoryService,
    SectionService,
    AuthGuardService,
    AuthService,
    StoreService,
    CommentService,
    SearchService,
    LoginService,
    RecommendService,
    { provide: HTTP_INTERCEPTORS, useClass: MyHttpInterceptor, multi: true },
  ],
  bootstrap: [AppComponent],
})
export class AppModule {}
