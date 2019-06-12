import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { chunk } from 'lodash';
import { combineLatest, BehaviorSubject } from 'rxjs';
import { map, switchMap, share, distinctUntilChanged, debounceTime } from 'rxjs/operators';

import { LocalAppService } from '../../services/local-app.service';
import { AuthService } from 'app/services/auth.service';
import { Software } from 'app/services/software.service';

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
    private authService: AuthService,
  ) {}
  readonly DisabledList = [
    'dde',
    'dde-control-center',
    'dde-introduction',
    'dde-file-manager',
    'deepin-appstore',
    'deepin-manual',
  ];
  readonly pageSize = 13;
  logged = this.authService.logged$;
  selected: string;
  removing: string[] = [];
  pageIndex$ = this.route.queryParamMap.pipe(map(query => Number(query.get('page') || 0)));
  result$ = this.pageIndex$.pipe(
    switchMap(pageIndex => this.localAppService.list({ pageSize: this.pageSize, pageIndex })),
    share(),
  );
  count$ = this.result$.pipe(map(result => Math.ceil(result.total / this.pageSize)));
  removingList$ = this.localAppService.removingList();

  remove(soft: Software) {
    this.selected = null;
    this.removing.push(soft.name);
    this.localAppService.removeLocal(soft);
  }

  ngOnInit() {}

  gotoPage(pageIndex: number) {
    this.router.navigate([], { queryParams: { page: pageIndex } });
  }
}
