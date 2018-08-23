import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import { AppVersion } from '../../models/app-version';
import { StoreService } from '../../services/store.service';

@Component({
  selector: 'dstore-job-button',
  templateUrl: './job-button.component.html',
  styleUrls: ['./job-button.component.scss'],
})
export class JobButtonComponent implements OnInit {
  constructor(private storeService: StoreService) {}
  @Input() appName: string;
  @Input() localName: string;
  @Input() version: AppVersion;
  @Input() openType: string;
  @Output() start = new EventEmitter<string>();
  canOpen = canOpen;
  disabled: boolean;

  ngOnInit() {}

  openApp(e: Event) {
    e.stopPropagation();
    switch (this.openType) {
      case 'desktop':
        this.storeService.openApp(this.appName);
        break;
    }
  }

  installApp(e: Event) {
    e.stopPropagation();
    this.storeService.installPackage(this.appName, this.localName).subscribe(job => {
      this.disabled = true;
      this.start.emit(job);
    });
  }

  updateApp(e: Event) {
    e.stopPropagation();
    this.storeService.updatePackage(this.appName, this.localName).subscribe(job => {
      this.disabled = true;
      this.start.emit(job);
    });
  }
}

const canOpen = ['desktop'];
