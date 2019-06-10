import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class DeepinidInfoService {
  private apiURL = environment.metadataServer + '/api/deepin_user';
  constructor(private http: HttpClient) {}

  getDeepinUserInfo(uid: number): Promise<DeepinInfo>;
  getDeepinUserInfo(uidList: Array<number>): Promise<Array<DeepinInfo>>;

  getDeepinUserInfo(param: number | Array<number>) {
    if (typeof param === 'number') {
      return this.http
        .get<DeepinInfo[]>(this.apiURL, { params: { uid: String(param) } })
        .toPromise()
        .then(users => users.find(user => user.uid === param));
    } else if (param instanceof Array) {
      return this.http
        .get<DeepinInfo[]>(this.apiURL, { params: { uid: param.map(String) } })
        .toPromise()
        .then(users => users);
    }
  }
}

export interface DeepinInfo {
  avatar: string;
  nickname: string;
  profile_image: string;
  region: string;
  uid: number;
  username: string;
}
