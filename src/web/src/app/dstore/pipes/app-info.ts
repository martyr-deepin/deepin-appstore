import { Pipe, PipeTransform } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { scan, debounceTime, share, flatMap, map, shareReplay } from 'rxjs/operators';

import { memoize } from 'lodash';

import { AppService } from 'app/services/app.service';

@Pipe({
  name: 'appInfo',
})
export class AppInfoPipe implements PipeTransform {
  private query$ = new Subject<string>();
  private result$ = this.getResult();
  constructor(private appService: AppService) {}
  getResult() {
    return this.query$.pipe(
      scan((list: string[], name: string) => [...list, name], []),
      debounceTime(10),
      flatMap(list => {
        return this.appService.getApps(list);
      }),
      share(),
    );
  }
  transform(name: string) {
    setTimeout(() => {
      this.query$.next(name);
    }, 0);
    return this.result$.pipe(
      map(list => {
        return list.find(app => app.name === name);
      }),
    );
  }
}
