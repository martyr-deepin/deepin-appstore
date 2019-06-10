import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterModule } from '@angular/router';

import { ClientModule } from 'app/modules/client/client.module';

import { DialogDirective } from './directives/dialog.directive';
import { HoverDirective } from './directives/hover.directive';
import { ResizeDirective } from './directives/resize.directive';
import { CoverDirective } from './directives/cover.directive';
import { CircleDirective } from './directives/circle.directive';

import { WaitComponent } from './components/wait/wait.component';
import { ScrollbarComponent } from './components/scrollbar/scrollbar.component';
import { AppTitleComponent } from './components/app-title/app-title.component';
import { AppListComponent } from './components/app-list/app-list.component';
import { CenterTitleComponent } from './components/center-title/center-title.component';
import { PaginatorComponent } from './components/paginator/paginator.component';
import { CloseButtonComponent } from './components/close-button/close-button.component';
import { ControlComponent } from './components/control/control.component';
import { StarComponent } from './components/star/star.component';
import { IndicationComponent } from './components/indication/indication.component';

import { RangePipe } from './pipes/range.pipe';
import { FitImage } from './pipes/fit-image';
import { FitLanguage } from './pipes/fit-lang';
import { SizeHuman } from './pipes/size-human';
import { DeepinidPipe } from './pipes/deepinid.pipe';

const components = [
  WaitComponent,
  ScrollbarComponent,
  AppTitleComponent,
  AppListComponent,
  PaginatorComponent,
  CenterTitleComponent,
  CloseButtonComponent,
  ControlComponent,
  StarComponent,
  IndicationComponent,
];
const directives = [DialogDirective, HoverDirective, ResizeDirective, CoverDirective];
const pipes = [RangePipe, FitImage, FitLanguage, SizeHuman, DeepinidPipe];
@NgModule({
  declarations: [...components, ...directives, ...pipes, CircleDirective],
  exports: [...components, ...directives, ...pipes, ClientModule],
  imports: [CommonModule, RouterModule, ClientModule],
})
export class ShareModule {}
