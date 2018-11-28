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
      .get<Result>(this.server + '/api/my/comment', {
        params: { page: page.toString(), count: pageSize.toString() },
      })
      .pipe(
        switchMap(
          result => this.appService.getApps(result.comment.map(c => c.appName)),
          (result, apps) => {
            result.comment = result.comment.map(c =>
              this.appService.addApp(c, apps.find(app => app.name === c.appName)),
            );
            return result;
          },
        ),
      );
  }
  create(appName: string, content: string, rate: number, version: string) {
    const c = {
      appName,
      content,
      rate,
      version,
    };
    return this.http.post(this.server + `/api/comment/app/${appName}`, c);
  }
  delete(id: number) {
    return this.http.delete(this.server + `/api/my/comment/${id}`);
  }
}

interface Result {
  comment: (UserComment & HasApp)[];
  totalCount: number;
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
