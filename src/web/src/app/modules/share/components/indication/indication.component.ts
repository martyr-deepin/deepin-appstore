import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'dstore-indication',
  templateUrl: './indication.component.html',
  styleUrls: ['./indication.component.scss'],
})
export class IndicationComponent implements OnInit {
  @Input() count: number;
  @Input() selectIndex = 0;
  @Output() change = new EventEmitter<number>();
  constructor() {}

  ngOnInit() {}

  get list() {
    return new Array(this.count).fill(null);
  }

  select(index: number) {
    this.change.emit(index);
    this.selectIndex = index;
    // selectIndex = $index;
  }
}
