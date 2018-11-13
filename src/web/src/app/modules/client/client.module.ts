import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { StoreService } from './services/store.service';
import { CircleButtonComponent } from './components/circle-button/circle-button.component';
import { JobButtonComponent } from './components/job-button/job-button.component';
import { ProgressButtonComponent } from './components/progress-button/progress-button.component';

@NgModule({
  imports: [CommonModule],
  providers: [StoreService],
  exports: [CircleButtonComponent, JobButtonComponent, ProgressButtonComponent],
  declarations: [CircleButtonComponent, JobButtonComponent, ProgressButtonComponent],
})
export class ClientModule {}
