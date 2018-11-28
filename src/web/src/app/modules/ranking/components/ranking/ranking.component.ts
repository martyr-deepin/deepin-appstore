import { Component, OnInit, ViewChild } from '@angular/core';
import { Observable } from 'rxjs';
import { startWith, map, switchMap } from 'rxjs/operators';

import {
  SortOrder,
  AppTitleComponent,
} from 'app/modules/share/components/app-title/app-title.component';

import { AppService, App } from 'app/services/app.service';

@Component({
  selector: 'dstore-ranking',
  templateUrl: './ranking.component.html',
  styleUrls: ['./ranking.component.scss'],
})
export class RankingComponent implements OnInit {
  constructor(private appService: AppService) {}
  @ViewChild(AppTitleComponent)
  appTitle: AppTitleComponent;
  limit = 100;
  apps$: Observable<App[]>;

  ngOnInit() {
    this.apps$ = this.appTitle.sortByChange.pipe(
      startWith(this.appTitle.sortBy),
      switchMap(
        () => {
          return this.appService.list();
        },
        (sort: SortOrder, apps: App[]) => {
          if (sort === SortOrder.Downloads) {
            apps = apps.sort((a, b) => b.downloads - a.downloads);
          } else if (sort === SortOrder.Score) {
            apps = apps.sort((a, b) => b.rate * b.ratings - a.rate * a.ratings);
          }
          // 预防无效应用,预读10个
          apps = apps.slice(0, this.limit + 10);
          return apps.map(app => app.name);
        },
      ),
      switchMap(appNameList => this.appService.getApps(appNameList)),
      map(apps => apps.slice(0, this.limit)),
    );
  }
}
