import { Injectable, NgZone } from '@angular/core';
import { Observable, merge } from 'rxjs';

import { Channel } from 'app/modules/client/utils/channel';
import { Subject } from 'rxjs';

@Injectable()
export class LoginService {
  constructor(private zone: NgZone) {
    Channel.connect<boolean>('menu.loginRequested').subscribe(status => {
      zone.run(() => {
        this.obs.next(status);
      });
    });
  }

  private obs = new Subject<boolean>();

  onOpenLogin(): Observable<boolean> {
    return this.obs.asObservable();
  }

  OpenLogin() {
    this.obs.next(true);
  }
  OpenLogout() {
    this.obs.next(false);
  }

  SetLoginStatue(logged: boolean) {
    Channel.exec('menu.setLoginState', logged);
  }
}
