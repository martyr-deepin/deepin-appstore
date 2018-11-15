import { Injectable, NgZone } from '@angular/core';
import { Observable, merge } from 'rxjs';

import { Channel } from 'app/modules/client/utils/channel';
import { Subject } from 'rxjs';
import { UserInfo, AuthService } from './auth.service';
import { DeepinUserInfoService } from './deepin-user-info.service';

@Injectable({
  providedIn: 'root',
})
export class LoginService {
  constructor(
    private zone: NgZone,
    private authService: AuthService,
    private deepinService: DeepinUserInfoService,
  ) {
    Channel.connect<boolean>('menu.loginRequested').subscribe(status => {
      this.zone.run(() => {
        this.obs.next(status);
      });
    });

    this.authService.auth$.subscribe(logged => {
      if (logged) {
        this.OpenLogout();
      } else {
        this.OpenLogin();
      }
    });
    this.authService.info$.subscribe(info => {
      this.SetLoginInfo(info);
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

  SetLoginInfo(userInfo: UserInfo) {
    if (userInfo) {
      // 设置头像
      this.deepinService.getDeepinUserInfo(userInfo.userID).then(info => {
        fetch(info.profile_image)
          .then(resp => resp.blob())
          .then(blob => {
            return new Promise<string>(resolve => {
              const reader = new FileReader();
              reader.onload = () => resolve(reader.result as string);
              reader.readAsDataURL(blob);
            });
          })
          .then(data => {
            Channel.exec('menu.setUserInfo', {
              name: info.username,
              uid: info.uid,
              profile_image: data.slice(data.indexOf(',') + 1),
            });
          });
      });
    } else {
      Channel.exec('menu.setUserInfo', {});
    }
  }
}
