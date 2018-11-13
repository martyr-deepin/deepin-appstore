import { Injectable, NgZone } from '@angular/core';
import { Observable } from 'rxjs';
import { map } from 'rxjs/operators';

import { Channel } from 'app/modules/client/utils/channel';
@Injectable()
export class SearchService {
  constructor(private zone: NgZone) {}

  onOpenApp(): Observable<string> {
    return new Observable(obs => {
      Channel.connect<string>('search.openApp').subscribe(app =>
        this.zone.run(() => {
          obs.next(app);
        }),
      );
    });
  }

  onOpenAppList(): Observable<SearchResult> {
    return new Observable(obs => {
      Channel.connect<[string, string[]]>('search.openAppList').subscribe(
        ([keyword, appNameList]) => {
          console.log(keyword, appNameList);
          this.zone.run(() => {
            obs.next({ keyword, appNameList });
          });
        },
      );
    });
  }
}

export interface SearchResult {
  keyword: string;
  appNameList: string[];
}
