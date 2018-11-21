import { Injectable } from '@angular/core';
import { Observable, fromEvent } from 'rxjs';
import { share, map } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class HoverService {
  constructor() {}
  private hover$ = fromEvent(window, 'mousemove').pipe(
    map(() => {
      return Array.from(document.querySelectorAll(':hover'));
    }),
    share(),
  );
  hoverElement(): Observable<Element[]> {
    return this.hover$;
  }
}
