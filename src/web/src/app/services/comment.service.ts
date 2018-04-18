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
      .map((resp: { comments: Comment[] }) => resp.comments);
  }
}

export interface Comment {
  id: number;
  appName: string;
  createTime: string;
  content: string;
  rate: number;
  version: string;
  user: User;
  ip: string;
  IsDeleted: boolean;
  likeCount: number;
}

interface User {
  id: number;
  name: string;
}
