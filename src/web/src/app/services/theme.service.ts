import { Injectable, NgZone } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { environment } from 'environments/environment';
import { Channel } from 'app/modules/client/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class ThemeService {
  private theme$ = new BehaviorSubject<string>(environment.themeName);
  constructor() {
    Channel.connect<string>('menu.switchThemeRequested').subscribe(theme => {
      this.theme$.next(theme);
    });
  }
  getTheme() {
    return this.theme$.asObservable();
  }
}
