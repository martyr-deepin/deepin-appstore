import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { switchMap } from 'rxjs/operators';

import { environment } from 'environments/environment';

import { AppService, HasApp } from 'app/services/app.service';

@Injectable({
  providedIn: 'root',
})
export class CommentsService {
  private server = environment.operationServer;
  constructor(private http: HttpClient, private appService: AppService) {}
  getComments(page: number, pageSize: number) {
    return this.http
      .get<Result>(this.server + '/api/user/my/comment', {
        params: { page: page.toString(), count: pageSize.toString() },
      })
      .pipe(
        switchMap(
          result =>
            this.appService.getApps(
              result.comment.map(c => c.appName),
              false,
              false,
            ),
          (result, apps) => {
            result.comment = result.comment.map(c =>
              this.appService.addApp(
                c,
                apps.find(app => app.name === c.appName),
              ),
            );
            return result;
          },
        ),
      );
  }
  create(c: CommentRequest) {
    return this.http.post(
      this.server + `/api/user/comment/app/${c.appName}`,
      c,
    );
  }
  delete(id: number) {
    return this.http.delete(this.server + `/api/user/my/comment/${id}`);
  }
  update(id: number, c: CommentRequest) {
    return this.http.patch(this.server + `/api/user/my/comment/${id}`, c);
  }
}

interface Result {
  comment: (UserComment & HasApp)[];
  totalCount: number;
}

interface CommentRequest {
  appName: string;
  content: string;
  rate: number;
  version: string;
}

export interface UserComment {
  id: number;
  appName: string;
  createTime: string;
  content: string;
  rate: number;
  version: string;
  userID: number;
  likeCount: number;
  likeByMe: boolean;
}
