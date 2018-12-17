import { Injectable, NgZone } from '@angular/core';
import { concat } from 'rxjs';
import { map, switchMap, shareReplay, filter, share } from 'rxjs/operators';

import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { Channel } from 'app/modules/client/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class AuthService {
  constructor(private zone: NgZone) {}

  info$ = concat(
    Channel.exec<UserInfo>('account.getUserInfo'),
    Channel.connect<UserInfo>('account.userInfoChanged'),
  ).pipe(
    map(info => {
      this.zone.run(() => {});
      return null;
      if (info && info.UserID) {
        console.log('welcome', info.UserID);
        return info;
      }
      return null;
    }),
    shareReplay(1),
  );
  logged$ = this.info$.pipe(map(info => info && info.IsLoggedIn));
  // get token
  getToken() {
    return Channel.exec<string>('account.getToken');
  }
  // 登录方法
  login() {
    console.log('login');
    Channel.exec('account.login');
  }
  // 登出方法
  logout() {
    console.log('logout');
    Channel.exec('account.logout');
  }
  // 需要验证事件
  authorized() {}
  // 打开注册页面
  register() {
    DstoreObject.openURL(`https://account.deepin.org/register`);
  }
}

export interface UserInfo {
  AccessToken: string;
  Expiry: number;
  HardwareID: string;
  IsLoggedIn: boolean;
  Token: string;
  UserID: number;
}
