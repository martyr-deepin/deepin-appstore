import { Injectable } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';

import * as _ from 'lodash';
import { map, first, switchMap } from 'rxjs/operators';
import { environment } from 'environments/environment';
import { AuthService } from 'app/services/auth.service';

@Injectable({
  providedIn: 'root',
})
export class CommentService {
  private server = environment.operationServer;
  constructor(private http: HttpClient, private auth: AuthService) {}

  list(appName: string, query?: { [key: string]: any }) {
    const params = Object.entries(query)
      .map(a => `${a[0]}=${encodeURIComponent(a[1])}`)
      .join('&');

    return this.auth.logged$.pipe(first()).pipe(
      map(logged => {
        if (logged) {
          return `${this.server}/api/user/comment/app/${appName}`;
        }
        return `${this.server}/api/comment/app/${appName}`;
      }),
      switchMap(url => {
        console.log(params);
        return this.http.get<CommentList>(url + '?' + params);
      }),
    );
  }

  create(appName: string, content: string, rate: number, version: string) {
    const c: Comment = {
      appName,
      content,
      rate,
      version,
    };
    return this.http.post(`${this.server}/api/user/comment/app/${appName}`, c);
  }

  own(appName: string, version: string) {
    return this.http
      .get<{ comment: Comment }>(
        `${
          this.server
        }/api/user/comment/app/${appName}/own?version=${encodeURIComponent(
          version,
        )}`,
      )
      .pipe(
        map(resp => {
          if (!resp.comment) {
            return undefined;
          }
          resp.comment.rate /= 2;
          return resp.comment;
        }),
      );
  }
  like(cid: number) {
    return this.http.post(`${this.server}/api/user/comment/like/${cid}`, null);
  }
  dislike(cid: number) {
    return this.http.post(
      `${this.server}/api/user/comment/dislike/${cid}`,
      null,
    );
  }
}

interface CommentList {
  comments: Comment[];
  hot: Comment[];
  totalCount: number;
}

export interface Comment {
  id?: number;
  appName: string;
  createTime?: string;
  content: string;
  rate: number;
  version: string;
  userID?: number;
  likeCount?: number;
  likeByMe?: boolean;
  hot?: boolean;
}

interface User {
  id: number;
  name: string;
}
