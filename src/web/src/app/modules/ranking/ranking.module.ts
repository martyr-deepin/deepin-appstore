import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { RankingRoutingModule } from './ranking-routing.module';
import { ShareModule } from 'app/modules/share/share.module';

import { RankingComponent } from './components/ranking/ranking.component';
import { CoverDirective } from './directives/cover.directive';

@NgModule({
  declarations: [RankingComponent, CoverDirective],
  imports: [CommonModule, RankingRoutingModule, ShareModule],
})
export class RankingModule {}
