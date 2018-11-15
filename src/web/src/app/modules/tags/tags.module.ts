import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { ShareModule } from 'app/modules/share/share.module';
import { TagsRoutingModule } from './tags-routing.module';
import { TagAppsComponent } from './components/tag-apps/tag-apps.component';

@NgModule({
  declarations: [TagAppsComponent],
  imports: [CommonModule, TagsRoutingModule, ShareModule],
})
export class TagsModule {}
