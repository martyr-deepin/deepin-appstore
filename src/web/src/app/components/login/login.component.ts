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
      if (!isLogin) {
        this.loginURL = this.domSanitizer.bypassSecurityTrustResourceUrl(
          'https://login.deepin.org/oauth2/logout?lang=' + navigator.language.split('-')[0],
        );
        this.authService.logout();
        return;
      }
      if (!this.dialogRef.nativeElement.open) {
        this.loginURL = this.domSanitizer.bypassSecurityTrustResourceUrl(
          BaseService.serverHosts.operationServer +
            '/api/oauthLogin/commenceLogin?' +
            Math.random(),
        );
        this.loaded = false;
        this.dialogRef.nativeElement.showModal();
      }
    });
  }
  // login iframe loading
  load(iframe: HTMLIFrameElement) {
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
    const [, token] = iframe.contentDocument.cookie
      .split('; ')
      .map(c => c.split('='))
      .find(([key, value]) => key === 'auth-token') || ['', ''];
    if (token) {
      this.dialogRef.nativeElement.close();
      this.authService.login(token);
    }
  }
}
