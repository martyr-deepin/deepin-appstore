import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { chunk } from 'lodash';
import { BehaviorSubject, combineLatest } from 'rxjs';
import { map } from 'rxjs/operators';

import { LocalAppService } from '../../services/local-app.service';
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
  installedList$ = this.chunkList();
  removeList$ = this.localAppService.RemoveList();
  pageIndex$ = this.route.queryParamMap.pipe(map(query => Number(query.get('page') || 1) - 1));
  ngOnInit() {}
  gotoPage(page: number) {
    this.router.navigate([], { queryParams: { page: page } });
  }
  chunkList() {
    return combineLatest(this.browserService.windowSize$, this.localAppService.LocalAppList()).pipe(
      map(([size, list]) => {
        // 使用窗口高度减去标题和分页宽度,再除于列表元素高度,计算出分页大小
        const pageSize = Math.floor((size.height - 64 - 20) / 64);
        return chunk(list, pageSize);
      }),
    );
  }
}
