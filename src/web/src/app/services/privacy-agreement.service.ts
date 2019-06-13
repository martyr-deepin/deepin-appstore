import { Injectable } from '@angular/core';
import { share } from 'rxjs/operators';
import { Channel } from 'app/modules/client/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class PrivacyAgreementService {
  private show = Channel.connect<void>('menu.privacyAgreementRequested').pipe(share());
  onShow() {
    return this.show;
  }
}
