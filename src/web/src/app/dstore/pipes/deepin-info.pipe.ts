import { Pipe, PipeTransform } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { environment } from 'environments/environment';
import { chunk } from 'lodash';

import { Observable, Subject, forkJoin } from 'rxjs';
import { map, bufferTime, filter, switchMap, first, share, tap } from 'rxjs/operators';

@Pipe({
  name: 'deepinInfo',
})
export class DeepinInfoPipe implements PipeTransform {
  apiURL = environment.metadataServer + '/api/deepin_user';

  buffer = new Subject<number>();
  result: Observable<DeepinInfo[]>;

  constructor(private http: HttpClient) {
    // 搜集参数，统一查询
    this.result = this.buffer.pipe(
      bufferTime(10),
      filter(list => list.length > 0),
      switchMap(list => {
        console.log(list);
        return forkJoin(
          chunk(list, 20)
            .map(arr => {
              let params = new HttpParams();
              arr.forEach(uid => (params = params.append('uid', uid.toString())));
              return params;
            })
            .map(params => this.http.get<DeepinInfo[]>(this.apiURL, { params })),
        ).pipe(map(arrList => [].concat(...arrList)));
      }),
      share(),
    );
  }
  transform(uid: number) {
    console.log('uid', uid);
    this.buffer.next(uid);
    return this.result.pipe(
      map(infoList => {
        return infoList.find(info => info.uid === uid);
      }),
    );
  }
}

export interface DeepinInfo {
  uid: number;
  username: string;
  profile_image: string;
}
