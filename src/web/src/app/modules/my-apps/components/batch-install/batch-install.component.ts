import { Component, OnInit, Input, ViewChild, ElementRef } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { switchMap, share, map, distinctUntilChanged, publishReplay, refCount } from 'rxjs/operators';

import { RemoteAppService } from './../../services/remote-app.service';
import { Software } from 'app/services/software.service';

@Component({
  selector: 'dstore-batch-install',
  templateUrl: './batch-install.component.html',
  styleUrls: ['./batch-install.component.scss'],
})
export class BatchInstallComponent implements OnInit {
  @ViewChild('dialog', { static: true })
  dialogRef: ElementRef<HTMLDialogElement>;
  pageSize = 33;
  batchInstall = new Map<string, Software>();

  pageIndex$ = new BehaviorSubject<number>(0);
  result$ = this.pageIndex$.pipe(
    distinctUntilChanged(),
    switchMap(pageIndex => this.remoteAppService.RemoteAppList(pageIndex + 1, this.pageSize)),
    publishReplay(1),
    refCount(),
  );
  length$ = this.result$.pipe(map(result => result.total));
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
  upgrade(app: Software): boolean {
    return app.package && (!app.package.localVersion || app.package.upgradable);
  }
  touch(app: Software) {
    if (!this.upgrade(app)) {
      return;
    }
    if (this.batchInstall.has(app.name)) {
      this.batchInstall.delete(app.name);
    } else {
      this.batchInstall.set(app.name, app);
    }
  }
  touchPage(apps: Software[]) {
    apps.forEach(app => {
      this.touch(app);
    });
  }
  selectPage(apps: Software[]) {
    apps.filter(app => this.upgrade(app)).forEach(app => this.batchInstall.set(app.name, app));
  }
  installAll() {
    this.remoteAppService.installApps([...this.batchInstall.values()]);
    this.hide();
  }
}
