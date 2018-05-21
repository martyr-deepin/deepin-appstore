import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { NgModule } from '@angular/core';
import { HTTP_INTERCEPTORS, HttpClientModule } from '@angular/common/http';

import { RoutingModule } from './routing/routing.module';
import { DstoreModule } from './dstore/dstore.module';

import { MyHttpInterceptor } from './services/http-interceptor';

import { AppService } from './services/app.service';
import { CategoryService } from './services/category.service';
import { SectionService } from './services/section.service';
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
import { PaginatorComponent } from './components/paginator/paginator.component';
import { NotifyComponent } from './components/notify/notify.component';

import { LodashPipe } from './pipes/lodash.pipe';
import { MoreComponent } from './components/more/more.component';

@NgModule({
  declarations: [
    AppComponent,
    AppDetailComponent,
    CategoryComponent,
    DownloadComponent,
    UninstallComponent,
    SideNavComponent,
    CategoryComponent,
    IndexComponent,
    DownloadComponent,
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
    PaginatorComponent,
    LodashPipe,
    NotifyComponent,
    MoreComponent,
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    DstoreModule,
    RoutingModule,
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
