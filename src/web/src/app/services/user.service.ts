import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { map, shareReplay } from 'rxjs/operators';

import { JwtHelperService } from '@auth0/angular-jwt';

import { AuthService } from './auth.service';
import { BaseService } from '../dstore/services/base.service';

@Injectable({
  providedIn: 'root',
})
export class UserService {
  constructor(private http: HttpClient, private auth: AuthService) {}
  server = BaseService.serverHosts.operationServer;
  jwtService = new JwtHelperService();
  userInfo(userName: string): Observable<UserInfo> {
    console.log(userName);
    return this.http
      .get<{ user: UserInfo }>(this.server + '/api/deepin_id/' + userName)
      .pipe(map(resp => resp.user), shareReplay());
  }
  myInfo(): Observable<UserInfo> {
    return new Observable<UserInfo>(sub => {
      this.userInfo(this.myName()).subscribe(info => sub.next(info));
    });
  }
  myName(): string {
    console.log('myName', this.auth.isLoggedIn, this.jwtService.decodeToken(this.auth.token));
    if (!this.auth.isLoggedIn) {
      return 'undefined';
    }
    const { username } = this.jwtService.decodeToken(this.auth.token);
    return username;
  }
}
interface UserInfo {
  uid: number;
  username: string;
  scope: string;
  profile_image: string;
  website: string;
  signature: string;
}
