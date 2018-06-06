import { Injectable } from '@angular/core';

import { DstoreObject } from '../dstore-client.module/utils/dstore-objects';
import { LoginService } from './login.service';

@Injectable()
export class AuthService {
  constructor(private loginService: LoginService) {
    this.loginService.SetLoginStatue(this.isLoggedIn);
  }

  tokenStorageKey = 'auth-token';
  get isLoggedIn(): boolean {
    const logged = this.token !== null;
    this.loginService.SetLoginStatue(logged);
    return logged;
  }
  get token(): string {
    return (
      sessionStorage.getItem(this.tokenStorageKey) || localStorage.getItem(this.tokenStorageKey)
    );
  }

  login(token: string) {
    this.loginService.SetLoginStatue(true);
    localStorage.setItem(this.tokenStorageKey, token);
    location.reload();
  }

  logout() {
    if (this.isLoggedIn) {
      this.loginService.SetLoginStatue(false);
      sessionStorage.removeItem(this.tokenStorageKey);
      localStorage.removeItem(this.tokenStorageKey);
      // location.reload();
      location.href = 'https://login.deepin.org/oauth2/logout?callback=' + escape(location.href);
    }
  }

  register() {
    DstoreObject.openURL(`https://account.deepin.org/register`);
  }
}
