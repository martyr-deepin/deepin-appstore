import { Component, OnInit, Input } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import * as _ from 'lodash';
import { Observable } from 'rxjs';
import { map, shareReplay } from 'rxjs/operators';

@Component({
  selector: 'app-paging',
  templateUrl: './app-paging.component.html',
  styleUrls: ['./app-paging.component.scss'],
})
export class AppPagingComponent implements OnInit {
  // 元素个数
  @Input()
  set count(count: number) {
    this._count = count;
    this.pageListObs = this.getPageList();
  }
  get count(): number {
    return Math.ceil(this._count / this.pageSize);
  }
  // 页个数
  _count: number;
  // 分页大小
  @Input() pageSize = 10;
  // 分页列表长度
  @Input() pageListSize = 7;

  page = 1;
  pageListObs: Observable<number[]>;

  constructor(private router: Router, private route: ActivatedRoute) {}

  ngOnInit() {
    this.pageListObs = this.getPageList();
  }

  getPageList(): Observable<number[]> {
    return this.route.paramMap.pipe(
      map(params => parseInt(params.get('page'), 10)),
      map(page => {
        this.page = page;
        const pageList = _.chain(1)
          .range(this.count + 1)
          .chunk(this.pageListSize)
          .map((ps: number[]) => {
            if (ps.length < this.pageSize && ps.includes(this.count)) {
              return _.range(
                this.count > this.pageListSize ? this.count - this.pageListSize : 1,
                this.count + 1,
              );
            }
            return ps;
          })
          .find((ps: number[]) => ps.includes(page))
          .value();
        return pageList;
      }),
      shareReplay(),
    );
  }

  goto(page: number) {
    this.router.navigate(['..', page], {
      relativeTo: this.route,
      queryParamsHandling: 'merge',
    });
  }
}
