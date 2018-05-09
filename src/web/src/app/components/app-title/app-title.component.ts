import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'app-app-title',
  templateUrl: './app-title.component.html',
  styleUrls: ['./app-title.component.scss'],
})
export class AppTitleComponent implements OnInit {
  constructor() {}
  sortOrderList = Object.values(SortOrder);
  @Input() title = '';
  @Input() count: number;
  @Input() sortBy = SortOrder.Downloads;

  ngOnInit() {}
}

export enum SortOrder {
  Downloads = 'Downloads',
  Score = 'Score',
}
