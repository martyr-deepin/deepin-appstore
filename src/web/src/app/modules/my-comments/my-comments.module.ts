import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { MyCommentsRoutingModule } from './my-comments-routing.module';
import { CommentsComponent } from './comments/comments.component';
import { ShareModule } from '../share/share.module';

@NgModule({
  declarations: [CommentsComponent],
  imports: [CommonModule, MyCommentsRoutingModule, ShareModule],
})
export class MyCommentsModule {}
