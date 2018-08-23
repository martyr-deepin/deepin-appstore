import { Injectable, NgZone } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { environment } from 'environments/environment';
import { Channel } from '../dstore-client.module/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class ThemeService {
  private theme$ = new BehaviorSubject<string>(environment.themeName);
  constructor() {
    Channel.registerCallback('menu.switchThemeRequested', theme => {
      this.theme$.next(theme);
    });
  }
  getTheme() {
    return this.theme$.asObservable();
  }
}
