import { BrowserModule } from '@angular/platform-browser';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { LOCALE_ID, NgModule } from '@angular/core';
import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';
import { JwtModule } from '@auth0/angular-jwt';

import { RoutingModule } from './routing/routing.module';
import { DstoreModule } from './dstore/dstore.module';

import { MyHttpInterceptor } from './services/http-interceptor';
import { Locale } from './utils/locale';

import { AppService } from './services/app.service';
import { CategoryService } from './services/category.service';
import { SectionService } from './services/section.service';
import { DownloadService } from './services/download.service';
import { AuthService } from './services/auth.service';
import { AuthGuardService } from './services/auth-guard.service';
import { CommentService } from './services/comment.service';
import { BaseService } from './dstore/services/base.service';
import { StoreService } from './services/store.service';
import { SearchService } from './services/search.service';
import { LoginService } from './services/login.service';
import { RecommendService } from './services/recommend.service';

import { AppComponent } from './app.component';
import { AppDetailComponent } from './components/app-detail/app-detail.component';
import { SideNavComponent } from './components/side-nav/side-nav.component';
import { CategoryComponent } from './components/category/category.component';
import { IndexComponent } from './components/index/index.component';
import { DownloadComponent } from './components/download-manage/download-manage.component';
import { UpdateComponent } from './components/update/update.component';
import { UninstallComponent } from './components/uninstall/uninstall.component';
import { RankingComponent } from './components/ranking/ranking.component';
import { AppListComponent } from './components/app-list/app-list.component';
import { AppTitleComponent } from './components/app-title/app-title.component';
import { AppCommentComponent } from './components/app-comment/app-comment.component';
import { SearchComponent } from './components/search/search.component';
import { LoginComponent } from './components/login/login.component';
import { TopicComponent } from './components/topic/topic.component';
import { RecommendComponent } from './components/recommend/recommend.component';
import { WaitComponent } from './components/wait/wait.component';

@NgModule({
  declarations: [
    AppComponent,
    AppDetailComponent,
    CategoryComponent,
    DownloadComponent,
    UninstallComponent,
    UpdateComponent,
    SideNavComponent,
    CategoryComponent,
    IndexComponent,
    DownloadComponent,
    UpdateComponent,
    UninstallComponent,
    RankingComponent,
    AppListComponent,
    AppTitleComponent,
    AppCommentComponent,
    SearchComponent,
    LoginComponent,
    TopicComponent,
    RecommendComponent,
    WaitComponent,
  ],
  imports: [
    BrowserModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    DstoreModule,
    RoutingModule,
    JwtModule.forRoot({
      config: {
        tokenGetter: () =>
          sessionStorage.getItem('auth-token') || localStorage.getItem('auth-token'),
        headerName: 'Access-Token',
        authScheme: '',
        whitelistedDomains: BaseService.whiteList,
      },
    }),
  ],
  providers: [
    AppService,
    CategoryService,
    SectionService,
    DownloadService,
    AuthGuardService,
    AuthService,
    StoreService,
    CommentService,
    SearchService,
    LoginService,
    RecommendService,
    {
      provide: LOCALE_ID,
      useValue: Locale.getPcp47Locale(),
    },
    { provide: HTTP_INTERCEPTORS, useClass: MyHttpInterceptor, multi: true },
  ],
  bootstrap: [AppComponent],
})
export class AppModule {}
