import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { ShareModule } from 'app/modules/share/share.module';
import { IndexRoutingModule } from './index-routing.module';

import { IndexComponent } from './index.component';
import { TopicComponent } from './components/topic/topic.component';
import { MoreComponent } from './components/more/more.component';
import { CoverComponent } from './components/cover/cover.component';
import { RankingComponent } from './components/ranking/ranking.component';
import { PhraseComponent } from './components/phrase/phrase.component';
import { AssembleComponent } from './components/assemble/assemble.component';
import { TitleComponent } from './components/title/title.component';

const components = [IndexComponent, TopicComponent, MoreComponent];

@NgModule({
  declarations: [
    ...components,
    CoverComponent,
    RankingComponent,
    PhraseComponent,
    AssembleComponent,
    TopicComponent,
    TitleComponent,
  ],
  imports: [CommonModule, IndexRoutingModule, ShareModule],
})
export class IndexModule {}
