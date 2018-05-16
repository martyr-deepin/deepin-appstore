import { Component, OnInit, Input, ViewChild, ElementRef } from '@angular/core';
import { trigger, state, style, animate, transition } from '@angular/animations';
import { DomSanitizer, SafeUrl } from '@angular/platform-browser';
import { Observable } from 'rxjs';
import * as _ from 'lodash';

import { AuthService } from '../../services/auth.service';
import { LoginService } from '../../services/login.service';
import { BaseService } from '../../dstore/services/base.service';
import { CommentService, Comment } from '../../services/comment.service';
import { UserService } from '../../services/user.service';
import { encodeUriQuery } from '@angular/router/src/url_tree';
import { shareReplay } from 'rxjs/operators';

@Component({
  selector: 'app-app-comment',
  templateUrl: './app-comment.component.html',
  styleUrls: ['./app-comment.component.scss'],
})
export class AppCommentComponent implements OnInit {
  constructor(
    private loginService: LoginService,
    private domSanitizer: DomSanitizer,
    private authService: AuthService,
    private commentService: CommentService,
    private userService: UserService,
  ) {}

  @Input() appName: string;
  @Input() version: string;
  operationServer = BaseService.serverHosts.operationServer;
  commentContext = '';
  submitError: string;
  content = '';
  rate = 0;

  commentList: Comment[];
  historyList: Comment[];
  get currentList() {
    if (this.select === 'current') {
      return this.commentList;
    } else {
      return this.historyList;
    }
  }
  _select = 'current';
  set select(s: string) {
    if (s === 'current') {
      this.getList();
    } else {
      this.getHistoryList();
    }
    this.page = 0;
    this._select = s;
  }
  get select(): string {
    return this._select;
  }
  get name() {
    return this.userService.myName();
  }
  page = 0;
  own$: Observable<Comment>;
  getUserInfo = _.memoize(userName => this.userService.userInfo(userName));

  ngOnInit() {
    this.getList();
    this.getHistoryList();
    this.own$ = this.commentService.own(this.appName, this.version).pipe(shareReplay());
  }
  getList() {
    this.commentService
      .list(this.appName, {
        version: this.version,
      })
      .subscribe(commentList => (this.commentList = commentList));
  }
  getHistoryList() {
    this.commentService
      .list(this.appName, {
        excludeVersion: this.version,
      })
      .subscribe(commentList => (this.historyList = commentList));
  }

  login() {
    this.loginService.OpenLogin();
  }

  get isLoggedIn(): boolean {
    return this.authService.isLoggedIn;
  }

  submitComment() {
    if (!this.content) {
      this.submitError = '请填写评论内容';
      return;
    }
    if (this.rate === 0) {
      this.submitError = '请选择评分';
      return;
    }
    this.commentService.create(this.appName, this.content, this.rate * 2, this.version).subscribe(
      () => {
        this.submitError = null;
        this.getList();
        this.select = 'current';
      },
      err => {
        this.submitError = '评论失败';
      },
    );
  }
  thumbUpClick(c: Comment) {
    if (c.likeByMe) {
      this.commentService.dislike(c.id).subscribe(() => {
        c.likeByMe = false;
        c.likeCount--;
      });
    } else {
      this.commentService.like(c.id).subscribe(() => {
        c.likeByMe = true;
        c.likeCount++;
      });
    }
  }
}
