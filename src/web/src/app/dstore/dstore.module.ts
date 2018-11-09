import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { HttpModule } from '@angular/http';
import { RouterModule } from '@angular/router';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { ClientModule } from '../dstore-client.module/client.module';

import { AppService } from './services/app.service';
import { CategoryService } from './services/category.service';
import { DownloadingService } from './services/downloading.service';

import { TitleComponent } from './components/title/title.component';
import { CarouselComponent } from './components/carousel/carousel.component';
import { CoverComponent } from './components/cover/cover.component';
import { RankingComponent } from './components/ranking/ranking.component';
import { PhraseComponent } from './components/phrase/phrase.component';
import { TopicComponent } from './components/topic/topic.component';
import { IconComponent } from './components/icon/icon.component';
import { AssembleComponent } from './components/assemble/assemble.component';

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
  imports: [CommonModule, HttpModule, RouterModule, FormsModule, ReactiveFormsModule, ClientModule],
  providers: [AppService, CategoryService, DownloadingService],
  declarations: [
    TitleComponent,
    CarouselComponent,
    CoverComponent,
    RankingComponent,
    PhraseComponent,
    TopicComponent,
    IconComponent,
    AssembleComponent,
    DevPipe,
    AppInfoPipe,
    AppSearchPipe,
    FitLanguage,
    FitImage,
    SizeHuman,
    StarComponent,
    IndicationComponent,
    HoverDirective,
    ChunkPipe,
    RangePipe,
    SanitizerPipe,
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
    AssembleComponent,
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
