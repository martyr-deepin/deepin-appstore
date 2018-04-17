import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { DomSanitizer, SafeUrl } from '@angular/platform-browser';
import { Observable } from 'rxjs/Observable';

import { AuthService } from '../../services/auth.service';
import { BaseService } from '../../dstore/services/base.service';

@Component({
  selector: 'app-app-comment',
  templateUrl: './app-comment.component.html',
  styleUrls: ['./app-comment.component.scss']
})
export class AppCommentComponent implements OnInit {
  operationServer: string;
  authUrl: SafeUrl;
  commentContext = '';
  constructor(
    private baseService: BaseService,
    private domSanitizer: DomSanitizer,
    private authService: AuthService
  ) {
    this.operationServer = this.baseService.serverHosts.operationServer;
    this.authUrl = this.domSanitizer.bypassSecurityTrustResourceUrl(
      this.operationServer + '/api/oauthLogin/commenceLogin'
    );
  }

  @ViewChild('dialog') dialog: ElementRef;
  dialogEl: HTMLDialogElement;
  iframeLoading = true;

  ngOnInit() {
    this.dialogEl = <HTMLDialogElement>this.dialog.nativeElement;
  }

  iframeLoad(event: Event) {
    console.log('iframeLoad');
    const iframeEl = <HTMLIFrameElement>event.target;
    const cBtn = <HTMLButtonElement>iframeEl.contentDocument.querySelector(
      '#close'
    );
    if (cBtn) {
      cBtn.onclick = () => this.dialogEl.close();
    }
    const token = iframeEl.contentDocument.cookie
      .split('; ')
      .map(c => c.split('='))
      .find(([key, value]) => key === 'auth-token');
    this.iframeLoading = false;
    if (token && token.length === 2) {
      this.dialogEl.close();
      this.authService.login(token[1]);
    }
  }

  get isLoggedIn() {
    return this.authService.isLoggedIn;
  }

  log(any) {
    console.log(any);
  }
}
