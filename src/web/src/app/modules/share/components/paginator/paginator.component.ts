import { Component, OnInit, Input, Output, EventEmitter, OnChanges } from '@angular/core';
import { chain } from 'lodash';

@Component({
  selector: 'app-paginator',
  templateUrl: './paginator.component.html',
  styleUrls: ['./paginator.component.scss'],
})
export class PaginatorComponent implements OnInit, OnChanges {
  constructor() {}
  @Input() length: number;
  @Input() size: number;
  @Input() count: number;
  get pageCount() {
    return this.count || Math.ceil(this.length / this.size);
  }
  @Input() pageIndex = 0;
  @Input() brother = 5;

  @Output() pageIndexChange = new EventEmitter<number>(true);

  pageList: number[] = [];

  ngOnInit() {
    this.pageIndexChange.subscribe(page => {
      this.pageIndex = page;
      this.pageList = this.getPageList();
    });
  }
  ngOnChanges() {
    this.pageList = this.getPageList();
  }
  getPageList() {
    let pls = chain(0)
      .range(this.pageCount)
      .chunk(this.brother)
      .find(ps => ps.includes(this.pageIndex))
      .value();
    if (!pls) {
      pls = chain(0)
        .range(this.pageCount)
        .value()
        .slice(0, this.brother);
      if (pls.length > 0) {
        this.pageIndexChange.emit(pls[pls.length - 1]);
      } else {
        this.pageIndexChange.emit(0);
      }
    }
    if (pls.length < this.brother && !pls.includes(0)) {
      for (let i = pls.length; i < this.brother; i++) {
        pls.unshift(pls[0] - 1);
      }
    }
    return pls || [];
  }
}
