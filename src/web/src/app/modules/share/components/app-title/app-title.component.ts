import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'app-app-title',
  templateUrl: './app-title.component.html',
  styleUrls: ['./app-title.component.scss'],
})
export class AppTitleComponent implements OnInit {
  constructor() {}
  SortOrder = SortOrder;
  @Input() title = '';
  @Input() top: number;
  @Input() count: number;
  @Input() sortBy = sessionStorage.getItem('sortBy') || SortOrder.Downloads;
  @Output() sortByChange = new EventEmitter<SortOrder>();
  @Input() sortHidden = false;
  ngOnInit() {}
  change(order: SortOrder) {
    sessionStorage.setItem('sortBy', order);
    this.sortBy = order;
    this.sortByChange.emit(order);
  }
}

export enum SortOrder {
  Downloads = 'Download',
  Score = 'Rate',
}
