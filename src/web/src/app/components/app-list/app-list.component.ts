import { Component, OnInit, Input, OnChanges } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import * as _ from 'lodash';

import { App } from '../../dstore/services/app';
import { SortOrder } from '../app-title/app-title.component';
import { DownloadService } from '../../services/download.service';

@Component({
  selector: 'app-app-list',
  templateUrl: './app-list.component.html',
  styleUrls: ['./app-list.component.scss']
})
export class AppListComponent implements OnChanges {
  @Input() apps: App[] = [];
  @Input() sortBy: SortOrder;

  constructor(private downloadService: DownloadService) {}

  _apps: App[] = [];
  appsObs: Observable<App[]>;

  ngOnChanges() {
    if (this.apps && this.apps.length === 0) {
      return;
    }
    this.appsObs = this.getList();
  }

  getList() {
    if (this.sortBy === SortOrder.Downloads) {
      return (this.appsObs = this.downloadService.list.map(ds =>
        _.orderBy(this.apps, [app => ds[app.name] || 0], ['desc'])
      ));
    } else {
      return Observable.of(this.apps);
    }
  }
}
