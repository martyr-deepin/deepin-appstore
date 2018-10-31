import { Pipe, PipeTransform } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { environment } from 'environments/environment';
import { chunk } from 'lodash';

import { Observable, Subject, forkJoin } from 'rxjs';
import { map, debounceTime, switchMap, first, share, scan } from 'rxjs/operators';

export interface DeepinInfo {
  uid: number;
  username: string;
  profile_image: string;
}

@Pipe({
  name: 'deepinInfo',
})
export class DeepinInfoPipe implements PipeTransform {
  private apiURL = environment.metadataServer + '/api/deepin_user';

  private buffer = new Subject<number>();
  private result: Observable<DeepinInfo[]>;

  constructor(private http: HttpClient) {
    // 多次查询转为单次批量查询
    this.result = this.buffer.pipe(
      scan((acc: number[], value: number) => [...acc, value], []),
      debounceTime(10),
      switchMap(list => this.getDeepinInfo(...list)),
      share(),
    );
  }

  // 批量获取Deepin Info
  getDeepinInfo(...uidList: number[]) {
    uidList = Array.from(new Set(uidList));
    // 接口只允许同时最多查询20个，把批量参数进行切分
    const reqList = chunk(uidList, 20)
      .map(arr =>
        arr.reduce((params, uid) => params.append('uid', uid.toString()), new HttpParams()),
      )
      .map(params => this.http.get<DeepinInfo[]>(this.apiURL, { params }));
    return forkJoin(reqList).pipe(map(arrList => [].concat(...arrList)));
  }

  // 将查询发送到缓存，等待批量查询，筛选结果
  transform(uid: number) {
    if (!uid) {
      return null;
    }
    setTimeout(() => this.buffer.next(uid), 0);
    return this.result.pipe(
      map(infoList => {
        return infoList.find(info => info.uid === uid);
      }),
      first(),
    );
  }
}
