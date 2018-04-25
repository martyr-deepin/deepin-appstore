import { Injectable, NgZone } from '@angular/core';
import { Observable } from 'rxjs/Observable';

import { Channel } from '../utils/channel';
import { Observer } from 'rxjs/Observer';
import { Subject } from 'rxjs/Subject';

@Injectable()
export class LoginService {
  constructor(private zone: NgZone) {}

  private obs = new Subject<boolean>();

  onOpenLogin(): Observable<boolean> {
    return Observable.merge(
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

  SetLoginStatue(logged: boolean) {
    Channel.exec('menu.setLoginState', logged);
  }
}
