import {
  HttpHandler,
  HttpRequest,
  HttpInterceptor,
  HttpErrorResponse,
  HttpEvent,
} from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, of, throwError } from 'rxjs';
import { catchError } from 'rxjs/operators';

import { MaterializeService } from '../dstore/services/materialize.service';
import { LoginService } from '../services/login.service';
import { AuthService } from '../services/auth.service';

@Injectable()
export class MyHttpInterceptor implements HttpInterceptor {
  constructor(
    private materializeService: MaterializeService,
    private authService: AuthService,
    private loginService: LoginService,
  ) {}
  intercept(req: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
    if (this.authService.isLoggedIn) {
      req = req.clone({ setHeaders: { 'Access-Token': this.authService.token } });
    }
    return next.handle(req).pipe(
      catchError((err: HttpErrorResponse, caught) => {
        switch (err.status) {
          case 0:
            this.materializeService.toastError('网络错误，请检查网络');
            break;
          case 401:
            this.materializeService.toastError('登录失效，请重新登录');
            if (this.authService.isLoggedIn) {
              this.authService.logout();
            } else {
              this.loginService.OpenLogin();
            }
            break;
          case 429:
            this.materializeService.toastError('访问过于频繁，请稍后重试');
            break;
          case 500:
            this.materializeService.toastError('服务器错误，请稍后重试');
            break;
          default:
            throw err.error.error;
        }
        // return of(null);
        return throwError(err);
      }),
    );
  }
}
