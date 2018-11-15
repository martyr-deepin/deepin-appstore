import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { DownloadRoutingModule } from './download-routing.module';
import { DownloadComponent } from './components/download/download.component';
import { ShareModule } from '../share/share.module';
import { StoreJobErrorComponent } from './components/store-job-error/store-job-error.component';

@NgModule({
  declarations: [DownloadComponent, StoreJobErrorComponent],
  imports: [CommonModule, DownloadRoutingModule, ShareModule],
})
export class DownloadModule {}
