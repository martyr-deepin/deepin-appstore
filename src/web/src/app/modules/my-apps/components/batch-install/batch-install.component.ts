import { Component, OnInit, Input, ViewChild, ElementRef } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import {
  switchMap,
  share,
  map,
  first,
  distinctUntilChanged,
  publish,
  retry,
  shareReplay,
  publishReplay,
  refCount,
} from 'rxjs/operators';

import { AppService, App } from 'app/services/app.service';
import { RemoteAppService } from './../../services/remote-app.service';

@Component({
  selector: 'dstore-batch-install',
  templateUrl: './batch-install.component.html',
  styleUrls: ['./batch-install.component.scss'],
})
export class BatchInstallComponent implements OnInit {
  @ViewChild('dialog')
  dialogRef: ElementRef<HTMLDialogElement>;
  pageSize = 33;
  batchInstall = new Map<string, App>();

  pageIndex$ = new BehaviorSubject<number>(0);
  result$ = this.pageIndex$.pipe(
    distinctUntilChanged(),
    switchMap(pageIndex =>
      this.remoteAppService.RemoteAppList(pageIndex + 1, this.pageSize),
    ),
    publishReplay(1),
    refCount(),
  );
  length$ = this.result$.pipe(map(result => result.totalCount));
  apps$ = this.result$.pipe(
    map(result => {
      return result.apps.map(apps => apps.app);
    }),
    share(),
  );

  constructor(private remoteAppService: RemoteAppService) {}

  ngOnInit() {}
  show() {
    this.batchInstall.clear();
    this.dialogRef.nativeElement.showModal();
  }
  hide() {
    this.dialogRef.nativeElement.close();
  }
  upgrade(app: App): boolean {
    return app.version && (!app.version.localVersion || app.version.upgradable);
  }
  touch(app: App) {
    if (!this.upgrade(app)) {
      return;
    }
    if (this.batchInstall.has(app.name)) {
      this.batchInstall.delete(app.name);
    } else {
      this.batchInstall.set(app.name, app);
    }
  }
  touchPage(apps: App[]) {
    apps.forEach(app => {
      this.touch(app);
    });
  }
  selectPage(apps: App[]) {
    apps
      .filter(app => this.upgrade(app))
      .forEach(app => this.batchInstall.set(app.name, app));
  }

  installAll() {
    this.remoteAppService.installApps([...this.batchInstall.values()]);
  }
}
