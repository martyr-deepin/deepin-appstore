import { Injectable } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { scan, debounceTime, flatMap, share, map } from 'rxjs/operators';
import { StoreService } from 'app/dstore-client.module/services/store.service';
import { AppVersion } from 'app/dstore-client.module/models/app-version';

@Injectable({
  providedIn: 'root',
})
export class VersionService {
  constructor(private storeService: StoreService) {}
  private query = new Subject<string>();
  private result = this.query.pipe(
    scan((acc: string[], name: string) => [...acc, name], []),
    debounceTime(10),
    flatMap(list => this.getAppVersion(list)),
    share(),
  );
  getAppVersion(appNameList: string[]): Observable<AppVersion[]>;
  getAppVersion(appName: string): Observable<AppVersion>;

  getAppVersion(param: string | string[]) {
    if (param instanceof Array) {
      return this.storeService.getVersion(param);
    } else if (typeof param === 'string') {
      setTimeout(() => {
        this.query.next(param);
      }, 0);
      return this.result.pipe(map(list => list.find(v => v.name === param)));
    }
  }
}
