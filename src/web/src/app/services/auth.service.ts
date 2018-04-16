import { Injectable } from '@angular/core';

@Injectable()
export class AuthService {
  private tokenStorageKey = 'auth-token';
  login(token: string) {
    localStorage.setItem(this.tokenStorageKey, token);
  }
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
}
