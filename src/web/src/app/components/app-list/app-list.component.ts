import { Component, OnInit, Input, OnChanges } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import * as _ from 'lodash';

import { DstoreApp } from '../../services/app.service';
import { SortOrder } from '../app-title/app-title.component';

@Component({
  selector: 'app-app-list',
  templateUrl: './app-list.component.html',
  styleUrls: ['./app-list.component.scss']
})
export class AppListComponent {
  @Input() apps: DstoreApp[] = [];
  @Input() sortBy: SortOrder;

  constructor() {}

  get appList(): DstoreApp[] {
    return _.sortBy(this.apps, [
      this.sortBy === SortOrder.Downloads ? 'downloads' : 'rate',
      'name'
    ]).reverse();
  }
}
