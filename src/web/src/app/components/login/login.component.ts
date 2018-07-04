import { Component, OnInit, ViewChild } from '@angular/core';
import { SafeUrl, DomSanitizer } from '@angular/platform-browser';

import { DstoreObject } from '../../dstore-client.module/utils/dstore-objects';
import { BaseService } from '../../dstore/services/base.service';
import { AuthService } from '../../services/auth.service';
import { LoginService } from '../../services/login.service';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.scss'],
})
export class LoginComponent implements OnInit {
  constructor(
    private domSanitizer: DomSanitizer,
    private authService: AuthService,
    private loginService: LoginService,
  ) {}

  @ViewChild('dialog') dialogRef: { nativeElement: HTMLDialogElement };
  loaded = false;
  loginURL: SafeUrl;

  ngOnInit() {
    this.loginURL = this.domSanitizer.bypassSecurityTrustResourceUrl('');
    this.loginService.onOpenLogin().subscribe(isLogin => {
      if (!this.dialogRef.nativeElement.open) {
        if (isLogin) {
          this.loginURL = this.domSanitizer.bypassSecurityTrustResourceUrl(
            `${BaseService.serverHosts.operationServer}/api/oauthLogin/commenceLogin?lang=${
              navigator.language.split('-')[0]
            }&rand=${Math.random()}`,
          );
        } else {
          this.loginURL = this.domSanitizer.bypassSecurityTrustResourceUrl(
            'https://login.deepin.org/oauth2/logout',
          );
        }
        this.loaded = false;
        this.dialogRef.nativeElement.showModal();
      }
    });
  }
  // login iframe loading
  load(iframe: HTMLIFrameElement) {
    console.log('loaded', iframe.contentWindow.location, iframe.contentDocument.body.innerText);
    const bodyText = iframe.contentDocument.body.innerText;
    switch (iframe.contentWindow.location.pathname) {
      case '/oauth2/authorize':
        if (
          bodyText.includes('loading....') ||
          bodyText.includes('ERR_INTERNET_DISCONNECTED') ||
          bodyText.includes('ERR_NAME_RESOLUTION_FAILED')
        ) {
          this.loaded = false;
        } else {
          this.loginInit(iframe);
        }
        break;
      case '/api/oauthLogin/finishLogin':
        this.finish(iframe);
        break;
      case '/oauth2/logout':
        this.logout();
        break;
      default:
    }
  }
  loginInit(iframe: HTMLIFrameElement) {
    this.loaded = true;
    const closeButton = iframe.contentDocument.getElementById('close');
    if (closeButton) {
      closeButton.addEventListener('click', () => {
        this.dialogRef.nativeElement.close();
      });
    }
    for (const id of ['signup', 'forget']) {
      const link = iframe.contentDocument.getElementById(id) as HTMLLinkElement;
      if (link) {
        console.log(link);
        link.addEventListener('click', (e: MouseEvent) => {
          e.preventDefault();
          DstoreObject.openURL(link.href);
        });
      }
    }
  }
  finish(iframe: HTMLIFrameElement) {
    const [, token] = iframe.contentDocument.cookie
      .split('; ')
      .map(c => c.split('='))
      .find(([key, value]) => key === 'auth-token') || ['', ''];
    if (token) {
      this.dialogRef.nativeElement.close();
      this.authService.login(token);
    }
  }
  logout() {
    this.dialogRef.nativeElement.close();
    this.authService.logout();
  }
}
