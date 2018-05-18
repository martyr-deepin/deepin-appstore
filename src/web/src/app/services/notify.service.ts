import { Injectable } from '@angular/core';
import { DomSanitizer, SafeHtml } from '@angular/platform-browser';

import { Subject } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class NotifyService {
  private notify$ = new Subject<Notify>();
  constructor(private sanitized: DomSanitizer) {
    setTimeout(() => {
      this.notice('公告：这是一个测试这是一个测试这是一个测试');
    }, 3000);
  }

  success(msg: string) {
    this.notify$.next({
      content: this.sanitized.bypassSecurityTrustHtml(
        `<img src="/assets/icons/ok.svg"><span style="color: green">${msg}</span>`,
      ),
      delay: 2000,
    });
  }

  error(err: string) {
    this.notify$.next({
      content: this.sanitized.bypassSecurityTrustHtml(
        `<img src="/assets/icons/failed.svg"><span style="color: red">${err}</span>`,
      ),
      delay: 3000,
    });
  }

  notice(notice: string) {
    this.notify$.next({
      content: this.sanitized.bypassSecurityTrustHtml(
        `<span style='color: green'>${notice}</span>`,
      ),
    });
  }

  obs() {
    return this.notify$.asObservable();
  }
}

export interface Notify {
  content: SafeHtml;
  delay?: number;
}
