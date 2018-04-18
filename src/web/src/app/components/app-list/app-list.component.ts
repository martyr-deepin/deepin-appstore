import { Component, OnInit, Input, OnChanges } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import * as _ from 'lodash';

import { App } from '../../services/app.service';
import { SortOrder } from '../app-title/app-title.component';

@Component({
  selector: 'app-app-list',
  templateUrl: './app-list.component.html',
  styleUrls: ['./app-list.component.scss'],
})
export class AppListComponent {
  @Input() apps: App[] = [];
  @Input() sortBy: SortOrder;

  constructor() {}

  get appList(): App[] {
    return _.sortBy(this.apps, [
      this.sortBy === SortOrder.Downloads ? 'downloads' : 'rate',
      'name',
    ]).reverse();
  }
}
