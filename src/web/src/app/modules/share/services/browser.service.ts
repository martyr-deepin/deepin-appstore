import { Injectable } from '@angular/core';
import { fromEvent } from 'rxjs';
import { shareReplay, startWith, map } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class BrowserService {
  public readonly windowSize$ = fromEvent(window, 'resize').pipe(
    map(() => this.windowSize()),
    startWith(this.windowSize()),
    shareReplay(1),
  );
  constructor() {}
  private windowSize() {
    return {
      width: window.innerWidth,
      height: window.innerHeight,
    };
  }
}
