import { Component, OnInit, Input, ViewChild, ElementRef } from '@angular/core';
import { DomSanitizer, SafeUrl } from '@angular/platform-browser';
import { Observable } from 'rxjs/Observable';

import { AuthService } from '../../services/auth.service';
import { BaseService } from '../../dstore/services/base.service';
import { CommentService, Comment } from '../../services/comment.service';

@Component({
  selector: 'app-app-comment',
  templateUrl: './app-comment.component.html',
  styleUrls: ['./app-comment.component.scss'],
})
export class AppCommentComponent implements OnInit {
  @Input() appName: string;
  operationServer: string;
  authUrl: SafeUrl;
  commentContext = '';

  constructor(
    private baseService: BaseService,
    private domSanitizer: DomSanitizer,
    private authService: AuthService,
    private commentService: CommentService,
  ) {
    this.operationServer = this.baseService.serverHosts.operationServer;
    this.authUrl = this.domSanitizer.bypassSecurityTrustResourceUrl(
      this.operationServer + '/api/oauthLogin/commenceLogin',
    );
  }
  commentListObs: Observable<Comment[]>;

  @ViewChild('dialog') dialog: { nativeElement: HTMLDialogElement };
  iframeLoading = true;
  ngOnInit() {
    // this.dialogEl = this.dialog.nativeElement;
    this.commentListObs = this.commentService.list(this.appName);
  }

  iframeLoad(event: Event) {
    console.log('iframeLoad');
    const iframeEl = <HTMLIFrameElement>event.target;
    const cBtn = <HTMLButtonElement>iframeEl.contentDocument.querySelector(
      '#close',
    );
    if (cBtn) {
      cBtn.onclick = () => this.dialog.nativeElement.close();
    }
    const token = iframeEl.contentDocument.cookie
      .split('; ')
      .map(c => c.split('='))
      .find(([key, value]) => key === 'auth-token');
    this.iframeLoading = false;
    if (token && token.length === 2) {
      this.dialog.nativeElement.close();
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
