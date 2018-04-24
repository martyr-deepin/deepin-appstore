import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs/Observable';
import * as ScrollIntoView from 'scroll-into-view/scrollIntoView';

import { App } from '../../dstore/services/app';
import { AppService } from '../../services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { CanvasUtil } from '../../utils/canvas-util';

@Component({
  selector: 'app-app-detail',
  templateUrl: './app-detail.component.html',
  styleUrls: ['./app-detail.component.scss'],
})
export class AppDetailComponent implements OnInit {
  constructor(private route: ActivatedRoute, private appService: AppService) {}

  metadataServer: string;
  appObs: Observable<App>;

  ngOnInit() {
    this.metadataServer = BaseService.serverHosts.metadataServer;
    this.appObs = this.route.paramMap.mergeMap(param => {
      const appName = param.get('appName');
      console.log('app name:', appName);
      return this.appService
        .getApp(appName)
        .do(app => console.log('app info:', app));
    });
  }

  screenshotClick(elID: string) {
    ScrollIntoView(document.getElementById(elID));
    // chrome version 61 support
    // (<HTMLElement>event.target).scrollIntoView({
    //   behavior: 'smooth',
    //   inline: 'center',
    //   block: 'nearest'
    // });
  }

  previewImage(img: HTMLImageElement) {
    window['dstore'].channel.objects.imageViewer.openBase64(
      CanvasUtil.getBase64Image(img),
    );
  }

  log(v) {
    console.log(v);
  }
}

interface Desc {
  Rate: number;
  Category: number;
  Version: string;
  Size: string;
  UpdateDate: Date;
  OfficialWebsite: string;
  Introduction: string;
}
