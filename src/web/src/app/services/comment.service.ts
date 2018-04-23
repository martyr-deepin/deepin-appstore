import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { BaseService } from '../dstore/services/base.service';

@Injectable()
export class CommentService {
  private server: string;
  constructor(private http: HttpClient, private base: BaseService) {
    this.server = this.base.serverHosts.operationServer;
  }

  list(appName: string) {
    return this.http
      .get(`${this.server}/api/comment/app/${appName}`)
      .map((resp: { comments: Comment[] }) => {
        resp.comments.map(comment => (comment.rate /= 2));
        return resp.comments;
      });
  }

  create(appName: string, content: string, rate: number) {
    const c: Comment = {
      appName,
      content,
      rate,
      version: '1.0',
    };
    return this.http.post(`${this.server}/api/comment/app/${appName}`, c);
  }

  own(appName) {
    return this.http.get(`${this.server}/api/comment/${appName}/own`);
  }
}

export interface Comment {
  appName: string;
  createTime?: string;
  content: string;
  rate: number;
  version: string;
  user?: User;
  likeCount?: number;
}

interface User {
  id: number;
  name: string;
}
