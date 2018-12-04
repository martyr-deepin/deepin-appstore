import { first, switchMap } from 'rxjs/operators';
import { AppService } from 'app/services/app.service';
import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import { AppVersion } from '../../models/app-version';
import { StoreService } from '../../services/store.service';

@Component({
  selector: 'dstore-job-button',
  templateUrl: './job-button.component.html',
  styleUrls: ['./job-button.component.scss'],
})
export class JobButtonComponent implements OnInit {
  constructor(private storeService: StoreService, private appService: AppService) {}
  @Input()
  appName: string;
  @Input()
  localName: string;
  @Input()
  version: AppVersion;
  @Input()
  openType: string;
  @Output()
  start = new EventEmitter<string>();
  canOpen = canOpen;
  disabled: boolean;

  ngOnInit() {}

  openApp(e: Event) {
    e.stopPropagation();
    switch (this.openType) {
      case 'desktop':
        this.appService
          .getApp(this.appName)
          .pipe(first())
          .subscribe(app => this.storeService.openApp(app));
        break;
    }
  }

  installApp(e: Event) {
    e.stopPropagation();
    console.log(this.version);
    this.appService
      .getApp(this.appName)
      .pipe(
        first(),
        switchMap(app => this.storeService.installPackages([app])),
      )
      .subscribe(job => {
        this.disabled = true;
        this.start.emit(job);
      });
  }

  updateApp(e: Event) {
    e.stopPropagation();
    this.appService
      .getApp(this.appName)
      .pipe(
        first(),
        switchMap(app => this.storeService.updatePackages([app])),
      )
      .subscribe(job => {
        this.disabled = true;
        this.start.emit(job);
      });
  }
}

const canOpen = ['desktop'];
