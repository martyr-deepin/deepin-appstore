import { Component, OnInit } from '@angular/core';
import {} from '@';
import { Observable } from 'rxjs/Observable';

import { BaseService } from './dstore/services/base.service';
import { AppService } from './services/app.service';
import { App } from './dstore/services/app';
import { Channel } from './utils/channel';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent implements OnInit {
  title = 'app';
  arr = new Array(100);

  constructor(
    private baseService: BaseService,
    private appService: AppService,
  ) {}

  ngOnInit(): void {
    if (this.baseService.isNative) {
      this.appService.list().subscribe((apps: App[]) => {
        const appStringList = JSON.stringify(apps);
        Channel.exec('search.updateAppList', appStringList);
      });
    }

    // Observable.create(obs => {
    //   window['dstore'].channel.objects.search.openApp.connect((app: string) =>
    //     obs.next(app),
    //   );
    // }).subscribe(appName => console.log(appName));
  }
}
