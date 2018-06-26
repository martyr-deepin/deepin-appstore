import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { map, skip } from 'rxjs/operators';

import { DstoreObject } from '../dstore-client.module/utils/dstore-objects';
import { LoginService } from './login.service';
import { JwtHelperService } from '@auth0/angular-jwt';

@Injectable()
export class AuthService {
  constructor(private loginService: LoginService) {
    this.token$.subscribe(token => {
      if (token) {
        this.loginService.SetLoginStatue(true);
        localStorage.setItem(this.tokenStorageKey, token);
      } else {
        this.loginService.SetLoginStatue(false);
        localStorage.removeItem(this.tokenStorageKey);
      }
    });
  }
  private tokenStorageKey = 'auth-token';
  private jwtService = new JwtHelperService();
  private tokenSubject = new BehaviorSubject<string>(localStorage.getItem(this.tokenStorageKey));

  token$ = this.tokenSubject.asObservable();
  logged$ = this.token$.pipe(map(token => token !== null));
  info$ = this.token$.pipe(map(token => this.jwtService.decodeToken(token) as UserInfo));

  login(token: string) {
    this.tokenSubject.next(token);
  }

  logout() {
    this.tokenSubject.next(null);
  }

  register() {
    DstoreObject.openURL(`https://account.deepin.org/register`);
  }
}

export interface UserInfo {
  username: string;
  userID: number;
}
