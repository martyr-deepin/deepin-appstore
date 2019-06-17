import { Injectable, NgZone } from '@angular/core';
import { Router } from '@angular/router';
import { AuthService } from './auth.service';
import { DeepinidInfoService } from '../modules/share/services/deepinid.service';
import { Channel } from 'app/modules/client/utils/channel';

@Injectable({
  providedIn: 'root',
})
export class MenuService {
  constructor(private router: Router, private auth: AuthService, private dService: DeepinidInfoService) {}
  serve() {
    // menu user info
    this.auth.info$.subscribe(async userInfo => {
      console.log('menu info', userInfo);
      if (!userInfo) {
        Channel.exec('menu.setUserInfo', {});
        return;
      }
      const dInfo = await this.dService.getDeepinUserInfo(userInfo.UserID);
      const avatar = await fetch(dInfo.profile_image).then(resp => resp.blob());
      console.log(avatar);
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
    this.connectToRouter('menu.donateRequested', '/my/donates');
  }

  connectToRouter(signal: string, url: string) {
    Channel.connect(signal).subscribe(() => {
      this.router.navigateByUrl(url);
    });
  }
}
