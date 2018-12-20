import { Injectable, NgZone } from '@angular/core';
import { Observable } from 'rxjs';
import { map } from 'rxjs/operators';

import { Channel } from 'app/modules/client/utils/channel';
@Injectable({
  providedIn: 'root',
})
export class SearchService {
  constructor(private zone: NgZone) {}

  onOpenApp(): Observable<string> {
    return Channel.connect<string>('search.openApp');
  }

  onOpenAppList(): Observable<SearchResult> {
    return new Observable(obs => {
      Channel.connect<[string, string[]]>('search.openAppList').subscribe(
        ([keyword, appNameList]) => {
          obs.next({ keyword, appNameList });
        },
      );
    });
  }
}

export interface SearchResult {
  keyword: string;
  appNameList: string[];
}
