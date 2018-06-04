import { Injectable } from '@angular/core';
import { DomSanitizer, SafeHtml } from '@angular/platform-browser';

import { Subject } from 'rxjs';

import { Notify, NotifyType, NotifyStatus } from './notify.model';
import { HttpClient } from '@angular/common/http';
import { BaseService } from '../dstore/services/base.service';

@Injectable({
  providedIn: 'root',
})
export class NotifyService {
  private notify$ = new Subject<Notify>();
  constructor(private sanitized: DomSanitizer, private http: HttpClient) {
    this.http
      .get(BaseService.serverHosts.operationServer + '/api/bulletin', { responseType: 'text' })
      .subscribe(body => {
        const { bulletin }: { bulletin: Bulletin } = JSON.parse(
          body,
          (k: string, v) => (k.includes('Time') ? new Date(v) : v),
        );
        const t = new Date();
        if (bulletin.startTime <= t && bulletin.endTime > t) {
          this.notify({
            type: NotifyType.Bulletin,
            status: NotifyStatus.Success,
            content: bulletin.content,
          });
        }
      });
  }

  notify(n: Notify) {
    console.log('notify', n);
    this.notify$.next(n);
  }
  success(t: NotifyType) {
    return this.notify({ status: NotifyStatus.Success, type: t, delay: 2000 });
  }
  error(t: NotifyType) {
    return this.notify({ status: NotifyStatus.Error, type: t, delay: 3000 });
  }

  obs() {
    return this.notify$.asObservable();
  }
}
interface Bulletin {
  topic: string;
  content: string;
  startTime: Date;
  endTime: Date;
}
