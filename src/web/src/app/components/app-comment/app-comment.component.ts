import { Component, OnInit, Input, ViewChild, ElementRef } from '@angular/core';
import { DomSanitizer, SafeUrl } from '@angular/platform-browser';
import { Observable } from 'rxjs/Observable';

import { AuthService } from '../../services/auth.service';
import { LoginService } from '../../services/login.service';
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
  commentContext = '';

  constructor(
    private loginService: LoginService,
    private domSanitizer: DomSanitizer,
    private authService: AuthService,
    private commentService: CommentService,
  ) {
    this.operationServer = BaseService.serverHosts.operationServer;
  }
  commentListObs: Observable<Comment[]>;

  ngOnInit() {
    this.commentListObs = this.commentService.list(this.appName);
    this.commentService.own(this.appName).subscribe(console.log);
  }

  login() {
    this.loginService.OpenLogin();
  }

  // iframeLoad(event: Event) {
  //   console.log('iframeLoad');
  //   const iframeEl = <HTMLIFrameElement>event.target;
  //   const cBtn = <HTMLButtonElement>iframeEl.contentDocument.querySelector(
  //     '#close',
  //   );
  //   if (cBtn) {
  //     cBtn.addEventListener('click', () =>
  //       console.log(this.dialog.nativeElement),
  //     );
  //   }
  //   const token = iframeEl.contentDocument.cookie
  //     .split('; ')
  //     .map(c => c.split('='))
  //     .find(([key, value]) => key === 'auth-token');
  //   this.iframeLoading = false;
  //   if (token && token.length === 2) {
  //     this.dialog.nativeElement.close();
  //     this.authService.login(token[1]);
  //   }
  // }

  get isLoggedIn(): boolean {
    return this.authService.isLoggedIn;
  }

  submitComment(content: string, rate: number) {
    this.commentService
      .create(this.appName, content, rate)
      .subscribe(null, null, () => {
        this.commentListObs = this.commentService.list(this.appName);
      });
  }

  log(any) {
    console.dir(any);
  }
}
