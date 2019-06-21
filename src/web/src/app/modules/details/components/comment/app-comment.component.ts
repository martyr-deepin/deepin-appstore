import { Component, OnInit, Input, OnChanges, ViewChild, ElementRef } from '@angular/core';
import { trigger, state, style, animate, transition } from '@angular/animations';
import { DomSanitizer } from '@angular/platform-browser';
import { forkJoin } from 'rxjs';
import * as _ from 'lodash';

import smoothScrollIntoView from 'smooth-scroll-into-view-if-needed';

import { AuthService, UserInfo } from 'app/services/auth.service';
import { CommentService, Comment } from '../../services/comment.service';
import { switchMap, filter, tap } from 'rxjs/operators';

@Component({
  selector: 'dstore-app-comment',
  templateUrl: './app-comment.component.html',
  styleUrls: ['./app-comment.component.scss'],
  animations: [
    trigger('myComment', [
      state('in', style({ transform: 'scaleY(1)', opacity: 1 })),
      transition('void => in', [
        style({
          transform: 'scaleY(0)',
          opacity: 0,
        }),
        animate(200),
      ]),
    ]),
  ],
})
export class AppCommentComponent implements OnInit, OnChanges {
  constructor(
    private domSanitizer: DomSanitizer,
    private authService: AuthService,
    private commentService: CommentService,
  ) {}
  @ViewChild('commentRef', { static: true }) commentRef: ElementRef<HTMLDivElement>;
  @Input()
  appName: string;
  @Input()
  version: string;

  loading = true;
  info: UserInfo;
  own: Comment;

  CommentError = CommentError;
  comment = {
    content: '',
    rate: 0,
    error: null,
  };
  haveNewComment = false;
  total = [0, 0];
  CommentType = CommentType;
  select = CommentType.News;
  list: Comment[];
  hot: Comment[];
  page = { index: 0, size: 20 };

  login = this.authService.login;
  logout = this.authService.logout;
  register = () => this.authService.register();

  ngOnInit() {}
  ngOnChanges() {
    this.init();
  }
  init() {
    this.getInfo();
    this.getCommentTotal();
    this.getOwn();
    this.getList();
  }

  getInfo() {
    this.authService.info$.subscribe(info => (this.info = info));
  }

  getOwn() {
    this.authService.info$
      .pipe(
        tap(() => {
          this.getList();
        }),
        filter(info => info != null),
        switchMap(() => this.commentService.own(this.appName, this.version)),
      )
      .subscribe(own => {
        this.own = own;
      });
  }

  getList() {
    this.loading = true;
    this.commentService
      .list(this.appName, {
        page: this.page.index + 1,
        count: this.page.size,
        [this.select === CommentType.News ? 'version' : 'excludeVersion']: this.version,
      })
      .subscribe(
        result => {
          if (this.page.index === 0 && result.hot) {
            const hot = _.sortBy(result.hot, ['likeCount', 'createTime']).reverse();
            hot.forEach(c => (c.hot = true));
            this.list = [...hot, ...result.comments];
          } else {
            this.list = result.comments;
          }
          this.total[this.select] = result.totalCount;
        },
        null,
        () => {
          this.loading = false;
        },
      );
  }

  getCommentTotal() {
    forkJoin(
      this.commentService.list(this.appName, {
        page: 1,
        count: 1,
        version: this.version,
      }),
      this.commentService.list(this.appName, {
        page: 1,
        count: 1,
        excludeVersion: this.version,
      }),
    ).subscribe(([news, history]) => {
      this.total = [news.totalCount, history.totalCount];
    });
  }

  selectType(type: CommentType) {
    this.select = type;
    this.page.index = 0;
    this.getList();
  }

  submitComment() {
    this.comment.content = this.comment.content.trim();
    if (!this.comment.content && this.comment.rate === 0) {
      this.comment.error = CommentError.AllInvalid;
      return;
    }
    if (!this.comment.content) {
      this.comment.error = CommentError.CommentInvalid;
      return;
    }
    if (this.comment.rate === 0) {
      this.comment.error = CommentError.RateInvalid;
      return;
    }
    this.commentService.create(this.appName, this.comment.content, this.comment.rate * 2, this.version).subscribe(
      () => {
        this.getOwn();
        this.selectType(CommentType.News);
        this.comment = {
          rate: 0,
          content: '',
          error: null,
        };
        this.haveNewComment = true;
      },
      err => {
        this.comment.error = CommentError.Failed;
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
  commentTop() {
    smoothScrollIntoView(this.commentRef.nativeElement, {
      block: 'start',
    });
  }
  scrollToTop() {
    smoothScrollIntoView(document.querySelector('.appInfo'), {
      block: 'start',
    });
  }
}
enum CommentType {
  News,
  History,
}
export enum CommentError {
  Unknown,
  RateInvalid,
  CommentInvalid,
  AllInvalid,
  Failed,
}
