import { map } from 'rxjs/operators';
import { Injectable } from '@angular/core';
import { Router, CanActivate } from '@angular/router';

import { AuthService } from './auth.service';

@Injectable({
  providedIn: 'root',
})
export class AuthGuardService implements CanActivate {
  constructor(private router: Router, private authService: AuthService) {}
  canActivate() {
    return this.authService.logged$.pipe(
      map(logged => {
        return logged || this.router.createUrlTree(['/']);
      }),
    );
  }
}
