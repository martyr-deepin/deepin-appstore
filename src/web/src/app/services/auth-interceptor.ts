import {
  HttpHandler,
  HttpRequest,
  HttpInterceptor,
  HttpErrorResponse,
  HttpEvent,
} from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, of, throwError } from 'rxjs';
import { catchError, take, first, switchMap, map } from 'rxjs/operators';

import { AuthService } from './auth.service';

@Injectable()
export class AuthInterceptor implements HttpInterceptor {
  constructor(private authService: AuthService) {}
  intercept(
    req: HttpRequest<any>,
    next: HttpHandler,
  ): Observable<HttpEvent<any>> {
    return this.authService.logged$.pipe(
      first(),
      switchMap(async logged => {
        if (!logged) {
          return req;
        }
        const token = await this.authService.getToken();
        return req.clone({ setHeaders: { 'Access-Token': token } });
      }),
      switchMap(authReq => next.handle(authReq)),
      catchError((err: HttpErrorResponse) => {
        console.error('AuthInterceptor', err);
        throw err;
      }),
    );
  }
  async clone(req: HttpRequest<any>) {
    const logged = await this.authService.logged$.pipe(first()).toPromise();
    if (!logged) {
      return req;
    }
    const token = await this.authService.getToken();
    return req.clone({ setHeaders: { 'Access-Token': token } });
  }
}
