import { Component, OnInit, Input } from '@angular/core';
import smoothScrollIntoView from 'smooth-scroll-into-view-if-needed';

import { BaseService } from 'app/dstore/services/base.service';
import { CanvasUtil } from 'app/utils/canvas-util';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';

@Component({
  selector: 'app-screenshot',
  templateUrl: './screenshot.component.html',
  styleUrls: ['./screenshot.component.scss'],
})
export class ScreenshotComponent implements OnInit {
  constructor() {}
  readonly cdnSuffix = '!small';
  readonly metadataServer = BaseService.serverHosts.metadataServer;
  @Input()
  screenshotList: string[];
  select = 0;
  loading = false;

  ngOnInit() {}

  goto(index: number) {
    this.select = index;
    const el = Array.from(document.getElementsByName('screenshot'))[this.select];
    if (el) {
      smoothScrollIntoView(el, {
        behavior: 'smooth',
        inline: 'center',
        block: 'nearest',
      });
    }
  }
  loaded(el: HTMLImageElement) {
    el.style.minWidth = 'auto';
  }
  error(el: HTMLImageElement) {
    if (el.src.endsWith(this.cdnSuffix)) {
      el.src = el.src.slice(0, el.src.indexOf(this.cdnSuffix));
    }
  }
  previewImage(index: number) {
    DstoreObject.imagesPreview(this.screenshotList.map(s => this.metadataServer + '/' + s), index);
  }
}
