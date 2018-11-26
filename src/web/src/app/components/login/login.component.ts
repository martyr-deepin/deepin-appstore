import { Component, OnInit, ViewChild } from '@angular/core';
import { SafeUrl, DomSanitizer } from '@angular/platform-browser';
import { HttpClient } from '@angular/common/http';

import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
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
    private http: HttpClient,
  ) {}
  @ViewChild('dialog') dialogRef: { nativeElement: HTMLDialogElement };
  isLogin: boolean;
  loaded = false;
  loginURL: SafeUrl;
  server = BaseService.serverHosts.operationServer;

  ngOnInit() {
    this.loginURL = this.domSanitizer.bypassSecurityTrustResourceUrl('');
    this.loginService.onOpenLogin().subscribe(isLogin => {
      if (!this.dialogRef.nativeElement.open) {
        this.isLogin = isLogin;
        if (isLogin) {
          this.loginURL = this.domSanitizer.bypassSecurityTrustResourceUrl(
            `${BaseService.serverHosts.operationServer}/api/oauthLogin/commenceLogin?lang=${
              navigator.language.split('-')[0]
            }&rand=${Math.random()}`,
          );
        } else {
          this.loginURL = this.domSanitizer.bypassSecurityTrustResourceUrl(
            'http://test.login.deepin.org/oauth2/logout',
          );
        }
        this.loaded = false;
        this.dialogRef.nativeElement.showModal();
      }
    });
  }
  // login iframe loading
  load(iframe: HTMLIFrameElement) {
    // console.log('loaded', iframe.contentWindow.location, iframe.contentDocument.body.innerText);
    iframe.contentDocument.body.addEventListener('mousewheel', e => {
      e.preventDefault();
    });
    const bodyText = iframe.contentDocument.body.innerText;
    switch (iframe.contentWindow.location.pathname) {
      case '/oauth2/authorize':
        if (!this.isLogin) {
          this.logout();
        }
        if (bodyText.includes('loading....') || bodyText.includes('Failed to load URL')) {
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
    }
  }

  loginInit(iframe: HTMLIFrameElement) {
    this.loaded = true;
    const closeButton = iframe.contentDocument.getElementById('close');
    if (closeButton) {
      closeButton.addEventListener('click', () => {
        if (this.dialogRef.nativeElement.open) {
          this.dialogRef.nativeElement.close();
        }
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
    console.log(iframe.contentDocument.cookie);
    const [, token] = iframe.contentDocument.cookie
      .split('; ')
      .map(c => c.split('='))
      .find(([key]) => key === 'auth-token') || ['', ''];
    if (token) {
      this.close();
      this.authService.login(token);
    }
  }

  close() {
    if (this.dialogRef.nativeElement.open) {
      this.dialogRef.nativeElement.close();
    }
  }

  logout() {
    this.http.post(this.server + '/api/logout', null).subscribe(null, null, () => {
      this.close();
      this.authService.logout();
    });
  }
}
