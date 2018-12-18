import { Injectable, NgZone } from '@angular/core';
import { Router } from '@angular/router';
import { AuthService } from './auth.service';
import { DeepinUserInfoService } from './deepin-user-info.service';
import { Channel } from 'app/modules/client/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class MenuService {
  constructor(
    private zone: NgZone,
    private router: Router,
    private auth: AuthService,
    private dService: DeepinUserInfoService,
  ) {}
  serve() {
    // menu user info
    this.auth.info$.subscribe(async userInfo => {
      if (!userInfo) {
        return;
      }
      console.log('setUserInfo');
      const dInfo = await this.dService.getDeepinUserInfo(userInfo.UserID);
      const avatar = await fetch(dInfo.profile_image).then(resp => resp.blob());
      const data = await new Promise<string>((resolve, reject) => {
        const reader = new FileReader();
        reader.onload = () => resolve(reader.result as string);
        reader.onerror = reject;
        reader.readAsDataURL(avatar);
      });
      Channel.exec('menu.setUserInfo', {
        name: dInfo.username,
        uid: dInfo.uid,
        profile_image: data.slice(data.indexOf(',') + 1),
      });
    });
    // bind route
    this.connectToRouter('menu.appsRequested', '/my/apps');
    this.connectToRouter('menu.commentRequested', '/my/comments');
    this.connectToRouter('menu.rewardRequested', '/my/donates');
  }

  connectToRouter(signal: string, url: string) {
    Channel.connect(signal).subscribe(() => {
      this.zone.run(() => {
        this.router.navigateByUrl(url);
      });
    });
  }
}
