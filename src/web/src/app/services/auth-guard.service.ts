import { tap } from 'rxjs/operators';
import { LoginService } from './login.service';
import { Injectable } from '@angular/core';
import { CanActivate } from '@angular/router';

import { AuthService } from './auth.service';

@Injectable({
  providedIn: 'root',
})
export class AuthGuardService implements CanActivate {
  constructor(private authService: AuthService, private loginService: LoginService) {}
  canActivate() {
    return this.authService.logged$.pipe(
      tap(logged => {
        if (!logged) {
          this.loginService.OpenLogin();
        }
      }),
    );
  }
}
