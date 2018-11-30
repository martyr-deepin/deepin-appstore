import {
  HttpHandler,
  HttpRequest,
  HttpInterceptor,
  HttpResponse,
  HttpEvent,
} from '@angular/common/http';
import { Injectable } from '@angular/core';
import * as localforage from 'localforage';
import { Observable, from, empty } from 'rxjs';
import { switchMap, catchError, tap, startWith, timeout } from 'rxjs/operators';

@Injectable()
export class CacheInterceptor implements HttpInterceptor {
  private cacheStorage = localforage.createInstance({ name: 'dstore-http-cache' });

  constructor() {}

  intercept(req: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
    return from(this.cacheStorage.getItem<any>(req.url)).pipe(
      switchMap(body => {
        const saveCache = tap((resp: HttpEvent<any>) => {
          if (resp instanceof HttpResponse) {
            if (resp.headers.has('cache-control')) {
              this.cacheStorage.setItem(req.url, resp.body);
            }
          }
        });
        if (!body) {
          return next.handle(req).pipe(
            saveCache,
            timeout(15000),
          );
        }
        const cacheResp = new HttpResponse({ body, status: 200, statusText: 'OK', url: req.url });
        return next.handle(req).pipe(
          saveCache,
          startWith(cacheResp),
          timeout(15000),
          catchError(err => {
            console.error('http error', err);
            return empty();
          }),
        );
      }),
    );
  }
}
