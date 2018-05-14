import { Injectable } from '@angular/core';

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
  }

  logout() {
    if (this.isLoggedIn) {
      this.loginService.SetLoginStatue(false);
      sessionStorage.removeItem(this.tokenStorageKey);
      localStorage.removeItem(this.tokenStorageKey);
      location.href = ('https://login.deepin.org/oauth2/logout?callback=' +
        window['escape'](location.href)) as string;
    }
  }
}
