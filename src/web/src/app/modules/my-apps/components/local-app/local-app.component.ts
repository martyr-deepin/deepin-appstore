import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { chunk, get } from 'lodash';
import { Subject, combineLatest } from 'rxjs';
import { map, tap, share, shareReplay } from 'rxjs/operators';

import { LocalAppService } from '../../services/local-app.service';
import { BrowserService } from 'app/modules/share/services/browser.service';
import { App } from 'app/services/app.service';

@Component({
  selector: 'dstore-local-app',
  templateUrl: './local-app.component.html',
  styleUrls: ['./local-app.component.scss'],
})
export class LocalAppComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private router: Router,
    private localAppService: LocalAppService,
    private browserService: BrowserService,
  ) {}

  removing: string[] = [];
  select: string;
  listHeight$ = new Subject<number>();
  pageSize$ = this.listHeight$.pipe(map(height => Math.floor(height / 64)));
  pageIndex$ = this.route.queryParamMap.pipe(map(query => Number(query.get('page') || 1) - 1));
  localApps$ = this.localAppService.LocalAppList().pipe(share());
  apps$ = combineLatest(this.localApps$, this.pageSize$, this.pageIndex$).pipe(
    map(([apps, size, index]) => {
      return apps.slice(size * index, size * (index + 1));
    }),
  );
  count$ = combineLatest(this.localApps$, this.pageSize$).pipe(
    map(([apps, size]) => {
      return Math.ceil(apps.length / size);
    }),
  );
  removingList$ = this.localAppService.RemovingList().pipe(
    tap(list => {
      if (list.includes(this.select)) {
        this.select = null;
      }
    }),
    map(list => list.concat(this.removing)),
    shareReplay(),
  );

  gotoPage(pageIndex: number) {
    this.router.navigate([], { queryParams: { page: pageIndex + 1 } });
  }

  remove(app: App) {
    this.removing.push(app.name);
    this.localAppService.RemoveLocalApp(app);
  }

  ngOnInit() {}
}
