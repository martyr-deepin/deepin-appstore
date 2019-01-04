import { Injectable, NgZone } from '@angular/core';
import { concat, BehaviorSubject } from 'rxjs';
import { map, shareReplay, tap } from 'rxjs/operators';
import { Router } from '@angular/router';

import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { Channel } from 'app/modules/client/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class AuthService {
  constructor(private router: Router) {
    Channel.exec<UserInfo>('account.getUserInfo').then(info => {
      this.userInfo$.next(info);
    });
    Channel.connect<UserInfo>('account.userInfoChanged').subscribe(info => {
      console.log('userinfo', info);
      this.userInfo$.next(info);
    });
    // 退出登录后,刷新当前页面,以进行路由守卫验证
    this.logged$.subscribe(logged => {
      if (!logged) {
        this.router.navigate(['./']);
      }
    });
  }
  private userInfo$ = new BehaviorSubject<UserInfo>(null);
  info$ = this.userInfo$.pipe(
    map(info => {
      if (!info || !info.UserID || !info.IsLoggedIn) {
        return null;
      }
      return info;
    }),
  );
  logged$ = this.info$.pipe(map(Boolean));
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
