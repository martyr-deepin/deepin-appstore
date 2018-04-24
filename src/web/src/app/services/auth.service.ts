import { Injectable } from '@angular/core';

@Injectable()
export class AuthService {
  private tokenStorageKey = 'auth-token';
  get isLoggedIn(): boolean {
    return this.token !== null;
  }
  get token(): string {
    return (
      sessionStorage.getItem(this.tokenStorageKey) ||
      localStorage.getItem(this.tokenStorageKey)
    );
  }
  constructor() {}

  login(token: string) {
    localStorage.setItem(this.tokenStorageKey, token);
  }

  logout() {
    if (this.isLoggedIn) {
      sessionStorage.removeItem(this.tokenStorageKey);
      localStorage.removeItem(this.tokenStorageKey);
      location.href = `https://login.deepin.org/oauth2/logout?callback=${
        location.href
      }`;
    }
  }
}
