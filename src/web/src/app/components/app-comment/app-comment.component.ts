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
  }

  login() {
    this.loginService.OpenLogin();
  }

  get isLoggedIn(): boolean {
    return this.authService.isLoggedIn;
  }

  submitComment(content: string, rate: number) {
    this.commentService.create(this.appName, content, rate).subscribe(null, null, () => {
      this.commentListObs = this.commentService.list(this.appName);
    });
  }
}
