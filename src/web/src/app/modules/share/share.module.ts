import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterModule } from '@angular/router';
import { DstoreModule } from 'app/dstore/dstore.module';
import { ClientModule } from 'app/modules/client/client.module';

import { WaitComponent } from './components/wait/wait.component';
import { ScrollbarComponent } from './components/scrollbar/scrollbar.component';

import { DialogDirective } from './directives/dialog.directive';
import { AppTitleComponent } from './components/app-title/app-title.component';
import { AppListComponent } from './components/app-list/app-list.component';
import { CenterTitleComponent } from './components/center-title/center-title.component';
import { PaginatorComponent } from './components/paginator/paginator.component';

const components = [
  WaitComponent,
  ScrollbarComponent,
  AppTitleComponent,
  AppListComponent,
  PaginatorComponent,
  CenterTitleComponent,
];
const directives = [DialogDirective];

@NgModule({
  declarations: [...components, ...directives],
  exports: [...components, ...directives, DstoreModule, ClientModule],
  imports: [CommonModule, RouterModule, DstoreModule, ClientModule],
})
export class ShareModule {}
