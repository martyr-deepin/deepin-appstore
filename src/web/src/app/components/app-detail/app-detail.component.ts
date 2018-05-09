import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable, timer, of, iif } from 'rxjs';
import { flatMap, map, tap, share } from 'rxjs/operators';
import * as ScrollIntoView from 'scroll-into-view/scrollIntoView';

import { App } from '../../dstore/services/app';
import { AppService } from '../../services/app.service';
import { BaseService } from '../../dstore/services/base.service';
import { CanvasUtil } from '../../utils/canvas-util';
import { StoreService } from '../../services/store.service';
import { AppJobStatus } from '../../services/store-job-info';

@Component({
  selector: 'app-app-detail',
  templateUrl: './app-detail.component.html',
  styleUrls: ['./app-detail.component.scss'],
})
export class AppDetailComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private appService: AppService,
    private storeService: StoreService,
  ) {}

  metadataServer = BaseService.serverHosts.metadataServer;
  open = this.storeService.openApp;

  AppJobStatus = AppJobStatus;
  status$: Observable<AppJobStatus>;
  size$: Observable<number>;
  appObs: Observable<App>;

  ngOnInit() {
    this.appObs = this.route.paramMap.pipe(
      flatMap(param => {
        const appName = param.get('appName');
        return this.appService.getApp(appName);
      }),
    );

    this.status$ = timer(0, 1000).pipe(
      flatMap(app => this.appObs),
      flatMap(app =>
        this.storeService
          .appInstalled(app.name)
          .pipe(
            flatMap(installed =>
              iif(
                () => installed,
                of(AppJobStatus.finish),
                this.storeService
                  .getJobByName(app.name)
                  .pipe(map(info => (info ? AppJobStatus.running : AppJobStatus.ready))),
              ),
            ),
          ),
      ),
      share(),
    );
    this.size$ = this.appObs.pipe(flatMap(app => this.storeService.appDownloadSize(app.name)));
  }

  install(appName: string) {
    this.storeService.installPackage(appName).subscribe();
  }

  screenshotClick(elID: string) {
    // chrome 61 support
    // const opt: ScrollIntoViewOptions = {
    //   behavior: 'smooth',
    //   inline: 'center',
    //   block: 'nearest',
    // };
    // (<HTMLElement>event.target).scrollIntoView(opt);

    ScrollIntoView(document.getElementById(elID));
  }

  previewImage(img: HTMLImageElement) {
    window['dstore'].channel.objects.imageViewer.openBase64(CanvasUtil.getBase64Image(img));
  }

  log(v) {
    console.log(v);
  }
}
