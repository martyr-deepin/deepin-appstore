import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { StoreService } from './services/store.service';
import { ProgressButtonComponent } from './components/progress-button/progress-button.component';

@NgModule({
  imports: [CommonModule],
  providers: [StoreService],
  exports: [ProgressButtonComponent],
  declarations: [ProgressButtonComponent],
})
export class ClientModule {}
