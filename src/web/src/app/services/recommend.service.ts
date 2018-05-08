import { Injectable, NgZone } from '@angular/core';
import { Observable, merge, Subject } from 'rxjs';

import { Channel } from '../utils/channel';
import { HttpClient } from '@angular/common/http';
import { BaseService } from '../dstore/services/base.service';

@Injectable({
  providedIn: 'root',
})
export class RecommendService {
  constructor(private zone: NgZone, private http: HttpClient) {}

  server = BaseService.serverHosts.operationServer;
  private obs = new Subject<void>();

  onOpenRecommend(): Observable<void> {
    return merge(
      this.obs,
      Observable.create(obs => {
        Channel.registerCallback('menu.recommendAppRequested', () => {
          this.zone.run(obs.next.bind(obs));
        });
      }),
    );
  }

  openRecommend() {
    this.obs.next(null);
  }

  recommendSubmit(r: Recommend) {
    return this.http.post(this.server + '/api/recommend', r).subscribe(resp => console.log(resp));
  }
}

export class Recommend {
  name = '';
  type = 'linux';
  authorization = '';
  homePage = '';
  downloadURL = '';
  extraInfo = '';
}
export enum RecommendType {
  linux = 'linux应用',
  windows = 'windows应用',
  webApp = 'WebApp',
  android = 'android应用',
}
