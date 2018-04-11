import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { Observable } from 'rxjs/Observable';
import 'rxjs/add/operator/map';

import { BaseService } from '../dstore/services/base.service';
import { AppService as DstoreAppService } from '../dstore/services/app.service';
import { App } from '../dstore/services/app';

@Injectable()
export class AppService {
  private server: string;
  private _list: Observable<App[]>;

  constructor(
    private http: HttpClient,
    private appService: DstoreAppService,
    private baseService: BaseService
  ) {
    this.server = this.baseService.serverHosts.operationServer;
    this._list = this.getList().shareReplay();
  }

  get list() {
    return this._list;
  }
  private getList() {
    return this.http
      .get(`${this.server}/api/app`)
      .flatMap((resp: { apps: string[] }) => {
        return this.appService
          .getAppList()
          .map(appList => appList.filter(app => resp.apps.includes(app.name)));
      });
  }
}
