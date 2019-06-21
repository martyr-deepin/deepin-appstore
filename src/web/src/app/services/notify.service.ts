import { Injectable, NgZone } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { ReplaySubject } from 'rxjs';
import { first } from 'rxjs/operators';

import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { Notify, NotifyType, NotifyStatus } from './notify.model';
import { AuthService } from './auth.service';
import { environment } from 'environments/environment';
@Injectable({
  providedIn: 'root',
})
export class NotifyService {
  private notify$ = new ReplaySubject<Notify>(1);
  constructor(private http: HttpClient, private auth: AuthService) {
    this.getBulletin();
    if (environment.native) {
      DstoreObject.clearArchives().subscribe(() => {
        this.success(NotifyType.Clear);
      });
    }
  }

  private getBulletin() {
    this.http
      .get(environment.operationServer + '/api/bulletin', {
        responseType: 'text',
      })
      .subscribe(body => {
        const { bulletin }: { bulletin: Bulletin } = JSON.parse(body, (k: string, v) =>
          k.includes('Time') ? new Date(v) : v,
        );
        const t = new Date();
        if (bulletin.startTime <= t && bulletin.endTime > t) {
          let content: string;
          if (navigator.language === 'zh-CN') {
            content = bulletin.contentZh;
            if (bulletin.contentZh.length === 0) {
              content = bulletin.contentEn;
            }
          } else {
            content = bulletin.contentEn;
            if (bulletin.contentEn.length === 0) {
              content = bulletin.contentZh;
            }
          }
          this.notify({
            type: NotifyType.Bulletin,
            status: NotifyStatus.Success,
            content: content,
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
    const n = { status: NotifyStatus.Error, type: t, delay: 3000 };
    if (t === NotifyType.Recommend || t === NotifyType.Reminder) {
      this.auth.logged$.pipe(first()).subscribe(logged => logged && this.notify(n));
      return;
    }
    return this.notify(n);
  }

  obs() {
    return this.notify$.asObservable();
  }
}
interface Bulletin {
  topicZh: string;
  topicEn: string;
  contentZh: string;
  contentEn: string;
  startTime: Date;
  endTime: Date;
}
