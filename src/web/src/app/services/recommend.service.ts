import { Injectable, NgZone } from '@angular/core';
import { Observable, merge, Subject } from 'rxjs';
import { tap } from 'rxjs/operators';

import { Channel } from '../dstore-client.module/utils/channel';
import { HttpClient } from '@angular/common/http';
import { BaseService } from '../dstore/services/base.service';

@Injectable({
  providedIn: 'root',
})
export class RecommendService {
  constructor(private zone: NgZone, private http: HttpClient) {
    Channel.connect<void>('menu.recommendAppRequested').subscribe(() => {
      zone.run(() => {
        this.openRecommend();
      });
    });
  }

  server = BaseService.serverHosts.operationServer;
  private obs = new Subject<void>();

  onOpenRecommend(): Observable<void> {
    return this.obs.asObservable();
  }

  openRecommend() {
    this.obs.next(null);
  }

  recommendSubmit(r: Recommend) {
    return this.http.post(this.server + '/api/recommend', r);
  }
}

export class Recommend {
  name = '';
  type = 1;
  authorization = '';
  homePage = '';
  downloadURL = '';
  extraInfo = '';
}
