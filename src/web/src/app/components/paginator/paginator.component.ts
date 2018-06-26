import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import * as _ from 'lodash';

@Component({
  selector: 'app-paginator',
  templateUrl: './paginator.component.html',
  styleUrls: ['./paginator.component.scss'],
})
export class PaginatorComponent implements OnInit {
  constructor() {}
  @Input() length: number;
  @Input() size: number;
  get count() {
    return Math.ceil(this.length / this.size);
  }
  @Input() pageIndex = 0;
  @Input() brother = 5;

  @Output() pageIndexChange = new EventEmitter<number>(true);

  ngOnInit() {
    this.pageIndexChange.subscribe(page => (this.pageIndex = page));
  }
  get pageList() {
    const pls = _
      .chain(0)
      .range(this.count)
      .chunk(this.brother)
      .find(ps => ps.includes(this.pageIndex))
      .value();
    if (pls.length < this.brother && !pls.includes(0)) {
      for (let i = pls.length; i < this.brother; i++) {
        pls.unshift(pls[0] - 1);
      }
    }
    return pls || [];
  }
}
