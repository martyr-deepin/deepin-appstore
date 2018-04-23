import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs/Observable';

import { App } from '../../dstore/services/app';
import { AppService } from '../../dstore/services/app.service';
import { BaseService } from '../../dstore/services/base.service';

@Component({
  selector: 'app-download',
  templateUrl: './download.component.html',
  styleUrls: ['./download.component.scss'],
})
export class DownloadComponent implements OnInit {
  metadataServer: string;
  constructor(private appService: AppService) {}

  appsObs: Observable<App[]>;
  ngOnInit() {
    this.metadataServer = BaseService.serverHosts.metadataServer;
    this.appsObs = this.getList();
  }
  getList() {
    return this.appService.getAppList().map(apps => apps.slice(0, 10));
  }
}
