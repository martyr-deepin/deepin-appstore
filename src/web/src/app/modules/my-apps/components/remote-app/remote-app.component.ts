import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { combineLatest, BehaviorSubject } from 'rxjs';
import { map, switchMap, share, distinctUntilChanged } from 'rxjs/operators';
import { RemoteAppService } from './../../services/remote-app.service';
import { App } from 'app/services/app.service';

@Component({
  selector: 'dstore-remote-app',
  templateUrl: './remote-app.component.html',
  styleUrls: ['./remote-app.component.scss'],
})
export class RemoteAppComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    public router: Router,
    private remoteAppService: RemoteAppService,
  ) {}
  listHeight$ = new BehaviorSubject<number>(0);
  pageSize$ = this.listHeight$.pipe(
    map(height => Math.floor(height / 64)),
    distinctUntilChanged(),
  );
  pageIndex$ = this.route.queryParamMap.pipe(
    map(query => Number(query.get('page') || 1) - 1),
  );
  result$ = combineLatest(this.pageIndex$, this.pageSize$).pipe(
    switchMap(([index, size]) =>
      this.remoteAppService.RemoteAppList(index + 1, size),
    ),
    share(),
  );
  apps$ = this.result$.pipe(map(result => result.apps));
  count$ = combineLatest(this.pageSize$, this.result$).pipe(
    map(([size, result]) => {
      return Math.ceil(result.totalCount / size);
    }),
  );
  gotoPage = (page: number) =>
    this.router.navigate([], { queryParams: { page: page + 1 } });

  ngOnInit() {}

  installApp(app: App) {
    this.remoteAppService.installApps([app]);
  }
}
