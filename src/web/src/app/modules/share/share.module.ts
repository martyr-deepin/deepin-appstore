import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { WaitComponent } from './components/wait/wait.component';
import { ScrollbarComponent } from './components/scrollbar/scrollbar.component';

import { DialogDirective } from './directives/dialog.directive';

const components = [WaitComponent, ScrollbarComponent];
const directives = [DialogDirective];

@NgModule({
  declarations: [...components, ...directives],
  exports: [...components, ...directives],
  imports: [CommonModule],
})
export class ShareModule {}
