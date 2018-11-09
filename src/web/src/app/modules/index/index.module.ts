import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { ShareModule } from 'app/modules/share/share.module';
import { IndexRoutingModule } from './index-routing.module';

import { IndexComponent } from './index.component';
import { TopicComponent } from './components/topic/topic.component';
import { MoreComponent } from './components/more/more.component';

const components = [IndexComponent, TopicComponent, MoreComponent];

@NgModule({
  declarations: [...components],
  imports: [CommonModule, IndexRoutingModule, ShareModule],
})
export class IndexModule {}
