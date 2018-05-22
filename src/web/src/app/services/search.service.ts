import { Injectable, NgZone } from '@angular/core';
import { Observable } from 'rxjs';

import { Channel } from '../dstore-client.module/utils/channel';
@Injectable()
export class SearchService {
  constructor(private zone: NgZone) {}

  onOpenApp(): Observable<string> {
    return Observable.create(obs => {
      Channel.registerCallback('search.openApp', (appName: string) => {
        this.zone.run(obs.next.bind(obs, appName));
      });
    });
  }

  onOpenAppList(): Observable<SearchResult> {
    return Observable.create(obs => {
      Channel.registerCallback('search.openAppList', (keyword: string, appNameList: string[]) => {
        this.zone.run(obs.next.bind(obs, { keyword, appNameList }));
      });
    });
  }
}

export interface SearchResult {
  keyword: string;
  appNameList: string[];
}
