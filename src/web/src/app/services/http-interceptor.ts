import {
  HttpHandler,
  HttpRequest,
  HttpInterceptor,
  HttpErrorResponse,
  HttpEvent,
} from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, of, throwError } from 'rxjs';
import { catchError, flatMap, map, tap, switchMap, take } from 'rxjs/operators';

import { MaterializeService } from '../dstore/services/materialize.service';
import { LoginService } from '../services/login.service';
import { AuthService } from '../services/auth.service';

@Injectable()
export class MyHttpInterceptor implements HttpInterceptor {
  constructor(private authService: AuthService, private loginService: LoginService) {}
  intercept(req: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
    return this.authService.token$.pipe(
      take(1),
      map(token => {
        return token == null ? req : req.clone({ setHeaders: { 'Access-Token': token } });
      }),
      switchMap(r => next.handle(r)),
      catchError((err: HttpErrorResponse, caught) => {
        switch (err.status) {
          case 401:
            console.log('登录超时');
            this.authService.logged$.pipe(take(1)).subscribe(b => {
              if (b) {
                this.loginService.OpenLogout();
              } else {
                this.loginService.OpenLogin();
              }
            });
            return of(null);
        }
        return throwError(err);
      }),
    );
  }
}
