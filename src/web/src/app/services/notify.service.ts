import { Injectable } from '@angular/core';
import { DomSanitizer, SafeHtml } from '@angular/platform-browser';

import { Subject } from 'rxjs';

import { Notify, NotifyType, NotifyStatus } from './notify.model';

@Injectable({
  providedIn: 'root',
})
export class NotifyService {
  private notify$ = new Subject<Notify>();
  constructor(private sanitized: DomSanitizer) {}

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
