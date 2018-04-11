import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import 'rxjs/add/operator/map';

import * as _ from 'lodash';

import { BaseService } from '../dstore/services/base.service';
import { App } from '../dstore/services/app';
import { AppDownloading } from '../dstore/services/downloading.service';

@Injectable()
export class DownloadService {
  private _appNameList: Observable<{ [key: string]: number }>;
  constructor(private baseService: BaseService, private http: HttpClient) {
    this._appNameList = this.getList().shareReplay(1);
  }

  get list() {
    return this._appNameList;
  }
  getList(): Observable<{ [key: string]: number }> {
    return this.http
      .get(`${this.baseService.serverHosts.operationServer}/api/downloading`)
      .map((result: { apps: { appName: string; count: number }[] }) => {
        return _.chain(result.apps)
          .keyBy('appName')
          .mapValues('count')
          .value();
      });
  }
}
