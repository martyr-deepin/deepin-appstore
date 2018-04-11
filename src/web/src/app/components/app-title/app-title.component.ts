import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'app-app-title',
  templateUrl: './app-title.component.html',
  styleUrls: ['./app-title.component.scss']
})
export class AppTitleComponent implements OnInit {
  @Input() title = '';
  @Input() count: number;
  @Input() sortBy: SortOrder;

  @Output() sort = new EventEmitter<SortOrder>();

  constructor() {}

  ngOnInit() {}

  get sortOrderList() {
    return Object.values(SortOrder);
  }
}

export enum SortOrder {
  Downloads = 'Downloads',
  Score = 'Score'
}
