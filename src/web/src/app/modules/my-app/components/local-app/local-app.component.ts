import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { chunk, get } from 'lodash';
import { BehaviorSubject, combineLatest } from 'rxjs';
import { map, tap, shareReplay } from 'rxjs/operators';

import { LocalAppService, LocalAppInfo } from '../../services/local-app.service';
import { BrowserService } from 'app/modules/share/services/browser.service';

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
  select: string;
  installedList$ = this.chunkList();
  removing: string[] = [];
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

  chunkList() {
    return combineLatest(
      this.browserService.windowSize$,
      this.localAppService.LocalAppList(),
      this.route.queryParamMap,
    ).pipe(
      map(([size, list, query]) => {
        const pageIndex = Number(query.get('page') || 1) - 1;
        // 使用窗口高度减去标题和分页宽度,再除于列表元素高度,计算出分页大小
        const pageSize = Math.floor((size.height - 64 - 20) / 64);
        const chunks = chunk(list, pageSize);
        return {
          list: chunks[pageIndex] || chunks[chunks.length - 1],
          page: pageIndex,
          total: chunks.length,
        };
      }),
    );
  }

  remove(app: LocalAppInfo) {
    this.removing.push(app.app.name);
    this.localAppService.RemoveLocalApp(app);
  }

  ngOnInit() {}
}
