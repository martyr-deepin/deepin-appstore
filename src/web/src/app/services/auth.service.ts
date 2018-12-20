import { Injectable, NgZone } from '@angular/core';
import { concat, BehaviorSubject } from 'rxjs';
import { map, shareReplay, tap } from 'rxjs/operators';

import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { Channel } from 'app/modules/client/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class AuthService {
  constructor(private zone: NgZone) {
    Channel.exec<UserInfo>('account.getUserInfo').then(info => {
      this.userInfo$.next(info);
    });
    Channel.connect<UserInfo>('account.userInfoChanged').subscribe(info => {
      console.log('userinfo', info);
      this.userInfo$.next(info);
    });
  }
  private userInfo$ = new BehaviorSubject<UserInfo>(null);
  info$ = this.userInfo$.pipe(
    map(info => {
      if (!info || !info.UserID) {
        return null;
      }
      return info;
    }),
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
