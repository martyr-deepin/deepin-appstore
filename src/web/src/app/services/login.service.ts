import { Injectable, NgZone } from '@angular/core';
import { Observable, merge } from 'rxjs';

import { Channel } from '../dstore-client.module/utils/channel';
import { Subject } from 'rxjs';

@Injectable()
export class LoginService {
  constructor(private zone: NgZone) {}

  private obs = new Subject<boolean>();

  onOpenLogin(): Observable<boolean> {
    return merge(
      this.obs,
      Observable.create(obs => {
        Channel.registerCallback('menu.loginRequested', isLogin => {
          this.zone.run(obs.next.bind(obs, isLogin));
        });
      }),
    );
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
