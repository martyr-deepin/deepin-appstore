import { Payment } from './../../details/services/donate.model';
import { distinctUntilChanged, map, switchMap, share, tap } from 'rxjs/operators';
import { BehaviorSubject, combineLatest } from 'rxjs';
import { ActivatedRoute, Router } from '@angular/router';
import { DonatesService } from './../donates.service';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'dstore-donates',
  templateUrl: './donates.component.html',
  styleUrls: ['./donates.component.scss'],
})
export class DonatesComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private router: Router,
    private donatesService: DonatesService,
  ) {}
  Payment = Payment;
  loaded = false;
  // 监听列表高度
  listHeight$ = new BehaviorSubject<number>(0);
  // 根据列表高度计算列表行数
  pageSize$ = this.listHeight$.pipe(
    map(height => Math.floor(height / 160)),
    distinctUntilChanged(),
  );
  // 监听当前页
  pageIndex$ = this.route.queryParamMap.pipe(map(query => Number(query.get('page') || 1) - 1));
  // 根据列表行数和页数的变动,拉取列表数据
  result$ = combineLatest(this.pageIndex$, this.pageSize$).pipe(
    switchMap(([pageIndex, pageSize]) => {
      this.loaded = false;
      return this.donatesService.donateList(pageIndex + 1, pageSize);
    }),
    share(),
    tap(() => {
      this.loaded = true;
    }),
  );
  donates$ = this.result$.pipe(map(result => result.donations));
  length$ = this.result$.pipe(map(result => result.totalCount));

  ngOnInit() {
    this.result$.subscribe(console.log);
  }
  gotoPage(pageIndex: number) {
    this.router.navigate([], { queryParams: { page: pageIndex + 1 } });
  }
}
