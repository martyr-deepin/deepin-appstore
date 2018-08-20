import { Component, OnInit, Input, Output, EventEmitter, OnChanges } from '@angular/core';

import * as _ from 'lodash';

@Component({
  selector: 'app-dstore-paging',
  templateUrl: './dstore-paging.component.html',
  styleUrls: ['./dstore-paging.component.css'],
})
export class DstorePagingComponent implements OnInit, OnChanges {
  // 总页数
  @Input() length: number;
  // 显示页数
  @Input() showSize = 5;
  // 当前页码
  @Input() page = 1;
  // 页码更改
  @Output() pageChange = new EventEmitter<number>();
  // 显示进度条
  @Input() progress = true;
  pageList: number[];
  constructor() {}

  ngOnInit() {
    console.log('ngOnInit');
    this.ngOnChanges();
  }
  ngOnChanges() {
    if (!this.length) {
      return [];
    }
    this.pageList = _.chain(1)
      .range(this.length + 1)
      .chunk(this.showSize)
      .find(ps => ps.includes(this.page))
      .value();
    if (
      this.pageList.length < this.showSize &&
      this.length > this.showSize &&
      this.pageList.includes(this.length)
    ) {
      this.pageList = _.range(this.length, this.length - this.showSize).reverse();
    }
  }
}
