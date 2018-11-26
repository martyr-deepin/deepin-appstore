import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { combineLatest, BehaviorSubject } from 'rxjs';
import { map, tap, share, shareReplay, distinctUntilChanged, debounceTime } from 'rxjs/operators';

import { LocalAppService } from '../../services/local-app.service';
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
  ) {}

  removing: string[] = [];
  select: string;
  listHeight$ = new BehaviorSubject<number>(0);
  pageSize$ = this.listHeight$.pipe(
    debounceTime(100),
    map(height => Math.floor(height / 64)),
    distinctUntilChanged(),
  );
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
