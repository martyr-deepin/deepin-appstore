import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';
import { DeepinInfo } from 'app/dstore/services/deepin-info.model';
import { UserInfo } from './auth.service';

@Injectable({
  providedIn: 'root',
})
export class DeepinUserInfoService {
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
