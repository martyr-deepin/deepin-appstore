import { Injectable } from '@angular/core';
import * as screenFull from 'screenfull';

@Injectable({
  providedIn: 'root',
})
export class KeyboardService {
  constructor() {}
  server() {
    const handles = new Map<string, KeyboardHandle>();
    handles.set('F11', this.F11);
    handles.set('Escape', this.Escape);
    window.addEventListener('keydown', (event: KeyboardEvent) => {
      const handle = handles.get(event.key);
      if (handle) {
        handle(event);
      } else {
        console.log(event);
      }
    });
  }
  Escape() {
    if (!screenFull) {
      return;
    }
    if (screenFull.isFullscreen) {
      screenFull.exit();
    }
  }
  F11() {
    if (!screenFull) {
      return;
    }
    if (screenFull.isFullscreen) {
      screenFull.exit();
    } else {
      screenFull.request(document.documentElement);
    }
  }
}
type KeyboardHandle = (event: KeyboardEvent) => void;
