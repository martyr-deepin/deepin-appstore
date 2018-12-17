import { Injectable, NgZone } from '@angular/core';
import { Channel } from 'app/modules/client/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class AutoInstallService {
  constructor(private zone: NgZone) {}
  setAutoInstall(auto: boolean) {
    return Channel.exec('settings.setAutoInstall', auto);
  }
  getAutoInstall() {
    return Channel.exec('settings.getAutoInstall').then(() =>
      this.zone.run(() => {}),
    );
  }
}
