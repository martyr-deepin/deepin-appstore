import { Injectable } from '@angular/core';
import { Subject, BehaviorSubject } from 'rxjs';
import * as JwtDecode from 'jwt-decode';
import { map, first, distinctUntilChanged } from 'rxjs/operators';

import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { LoginService } from './login.service';
import { BaseService } from '../dstore/services/base.service';
import { environment } from 'environments/environment';

export interface UserInfo {
  username: string;
  userID: number;
}
console.log(environment);
@Injectable()
export class AuthService {
  constructor() {}

  private tokenStorageKey = 'auth-token:' + BaseService.domainName;
  private tokenSubject = new BehaviorSubject<string>(localStorage.getItem(this.tokenStorageKey));
  private loginSubject = new Subject<boolean>();

  token$ = this.tokenSubject.pipe(distinctUntilChanged());
  logged$ = this.token$.pipe(map(Boolean));
  info$ = this.token$.pipe(map(token => (token ? JwtDecode<UserInfo>(token) : null)));
  auth$ = this.loginSubject.asObservable();
  // 登录方法
  login(token: string) {
    localStorage.setItem(this.tokenStorageKey, token);
    this.tokenSubject.next(token);
  }
  // 登出方法
  logout() {
    localStorage.removeItem(this.tokenStorageKey);
    this.tokenSubject.next(null);
  }
  // 需要验证事件
  authorized() {
    this.logged$.pipe(first()).subscribe(logged => this.loginSubject.next(logged));
  }
  // 打开注册页面
  register() {
    DstoreObject.openURL(`https://account.deepin.org/register`);
  }
}
