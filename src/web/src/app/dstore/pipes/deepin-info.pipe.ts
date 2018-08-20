import { Pipe, PipeTransform } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { memoize } from 'lodash';
import { Observable } from 'rxjs';
import { map, shareReplay } from 'rxjs/operators';

import { BaseService } from '../services/base.service';

@Pipe({
  name: 'deepinInfo',
})
export class DeepinInfoPipe implements PipeTransform {
  constructor(private http: HttpClient) {}
  transform = memoize(
    (uid: number): Observable<DeepinInfo> => {
      return this.http
        .get<DeepinInfo[]>(BaseService.serverHosts.metadataServer + '/api/deepin_user', {
          params: { uid: uid.toString() },
        })
        .pipe(
          map(resp => resp.find(info => info.uid === uid)),
          shareReplay(),
        );
    },
  );
}

export interface DeepinInfo {
  uid: number;
  username: string;
  profile_image: string;
}
