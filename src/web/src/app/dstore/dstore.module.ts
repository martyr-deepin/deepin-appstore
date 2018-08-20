import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpModule } from '@angular/http';
import { RouterModule } from '@angular/router';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { ClientModule } from '../dstore-client.module/client.module';

import { AppService } from './services/app.service';
import { CategoryService } from './services/category.service';
import { DownloadingService } from './services/downloading.service';
import { MaterializeService } from './services/materialize.service';

import { TitleComponent } from './components/title/title.component';
import { CarouselComponent } from './components/carousel/carousel.component';
import { CoverComponent } from './components/cover/cover.component';
import { RankingComponent } from './components/ranking/ranking.component';
import { PhraseComponent } from './components/phrase/phrase.component';
import { TopicComponent } from './components/topic/topic.component';
import { IconComponent } from './components/icon/icon.component';
import { AssembleComponent } from './components/assemble/assemble.component';

import { AppNavComponent } from './web-components/app-nav/app-nav.component';
import { AppPagingComponent } from './web-components/app-paging/app-paging.component';
import { DstorePagingComponent } from './web-components/dstore-paging/dstore-paging.component';
import { AppSearchComponent } from './web-components/app-search/app-search.component';
import { ImageUploadComponent } from './web-components/image-upload/image-upload.component';
import { ProgressComponent } from './web-components/progress/progress.component';
import { StarComponent } from './widget/star/star.component';
import { IndicationComponent } from './widget/indication/indication.component';

import { HoverDirective } from './directive/hover.directive';

import { DevPipe } from './pipes/dev';
import { AppInfoPipe } from './pipes/app-info';
import { AppSearchPipe } from './pipes/app-search';
import { FitLanguage } from './pipes/fit-lang';
import { FitImage } from './pipes/fit-image';
import { SizeHuman } from './pipes/size-human';
import { ChunkPipe } from './pipes/chunk.pipe';
import { RangePipe } from './pipes/range.pipe';
import { SanitizerPipe } from './pipes/sanitizer.pipe';
import { DeepinInfoPipe } from './pipes/deepin-info.pipe';
import { StopDirective } from './directive/stop.directive';
import { IconPipe } from './pipes/icon.pipe';

@NgModule({
  imports: [
    BrowserModule,
    HttpModule,
    RouterModule,
    FormsModule,
    ReactiveFormsModule,
    ClientModule,
  ],
  providers: [AppService, CategoryService, DownloadingService, MaterializeService],
  declarations: [
    TitleComponent,
    CarouselComponent,
    CoverComponent,
    RankingComponent,
    PhraseComponent,
    TopicComponent,
    IconComponent,
    AppNavComponent,
    AppPagingComponent,
    AppSearchComponent,
    AssembleComponent,
    ImageUploadComponent,
    DevPipe,
    AppInfoPipe,
    AppSearchPipe,
    FitLanguage,
    FitImage,
    SizeHuman,
    ProgressComponent,
    StarComponent,
    IndicationComponent,
    HoverDirective,
    ChunkPipe,
    RangePipe,
    SanitizerPipe,
    DstorePagingComponent,
    DeepinInfoPipe,
    StopDirective,
    IconPipe,
  ],
  exports: [
    TitleComponent,
    CarouselComponent,
    CoverComponent,
    RankingComponent,
    PhraseComponent,
    TopicComponent,
    IconComponent,
    AppNavComponent,
    AppPagingComponent,
    DstorePagingComponent,
    AppSearchComponent,
    AssembleComponent,
    ImageUploadComponent,
    ProgressComponent,
    StarComponent,
    IndicationComponent,
    HoverDirective,
    DevPipe,
    AppInfoPipe,
    AppSearchPipe,
    FitLanguage,
    FitImage,
    SizeHuman,
    ChunkPipe,
    RangePipe,
    DeepinInfoPipe,
    IconPipe,
  ],
})
export class DstoreModule {}
