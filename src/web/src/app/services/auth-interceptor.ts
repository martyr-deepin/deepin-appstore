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
    return this.authService.token$.pipe(
      first(),
      map(token =>
        !token ? req : req.clone({ setHeaders: { 'Access-Token': token } }),
      ),
      switchMap(authReq => next.handle(authReq)),
      catchError((err: HttpErrorResponse) => {
        console.error('AuthInterceptor', err);
        throw err;
      }),
    );
  }
}
