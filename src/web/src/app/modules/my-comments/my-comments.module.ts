import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { MyCommentsRoutingModule } from './my-comments-routing.module';
import { ShareModule } from '../share/share.module';
import { CommentsComponent } from './components/comments/comments.component';
import { EditComponent } from './components/edit/edit.component';

@NgModule({
  declarations: [CommentsComponent, EditComponent],
  imports: [CommonModule, MyCommentsRoutingModule, ShareModule, FormsModule],
})
export class MyCommentsModule {}
