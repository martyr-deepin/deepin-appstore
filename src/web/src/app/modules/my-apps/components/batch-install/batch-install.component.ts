import { Component, OnInit, Input, ViewChild, ElementRef } from '@angular/core';
import { chunk } from 'lodash';
import { App } from 'app/services/app.service';
import { BrowserService } from 'app/modules/share/services/browser.service';

@Component({
  selector: 'dstore-batch-install',
  templateUrl: './batch-install.component.html',
  styleUrls: ['./batch-install.component.scss'],
})
export class BatchInstallComponent implements OnInit {
  @ViewChild('dialog')
  dialogRef: ElementRef<HTMLDialogElement>;
  @Input()
  set apps(apps: App[]) {
    this.length = apps.length;
    this.chunks = chunk(apps, 45);
  }
  chunks: App[][] = [];
  length = 0;
  page = 0;
  batchInstall = new Set<string>();
  constructor(private browserService: BrowserService) {}
  ngOnInit() {}
  show() {
    this.page = 0;
    this.batchInstall.clear();
    this.dialogRef.nativeElement.showModal();
  }
  hide() {
    this.dialogRef.nativeElement.close();
  }
  click(appName: string) {
    if (this.batchInstall.has(appName)) {
      this.batchInstall.delete(appName);
    } else {
      this.batchInstall.add(appName);
    }
  }
  addAll() {
    this.chunks[this.page].forEach(app => this.batchInstall.add(app.name));
  }
  clickAll() {
    this.chunks[this.page].forEach(app => {
      if (this.batchInstall.has(app.name)) {
        this.batchInstall.delete(app.name);
      } else {
        this.batchInstall.add(app.name);
      }
    });
  }
}
