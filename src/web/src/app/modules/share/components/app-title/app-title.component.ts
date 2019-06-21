import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-app-title',
  templateUrl: './app-title.component.html',
  styleUrls: ['./app-title.component.scss'],
})
export class AppTitleComponent implements OnInit {
  constructor(private router: Router, private route: ActivatedRoute) {}
  SortOrder = SortOrder;
  @Input() title = '';
  @Input() top: number;
  @Input() count: number;
  @Input() sortBy = SortOrder.Downloads;
  @Output() sortByChange = new EventEmitter<SortOrder>();
  @Input() sortHidden = false;
  @Input() useRouterQuery = false;
  ngOnInit() {
    if (this.useRouterQuery) {
      this.sortBy =
        (this.route.snapshot.queryParamMap.get('order') as any) ||
        SortOrder.Downloads;
    }
  }
  change(order: SortOrder) {
    this.sortBy = order;
    this.sortByChange.emit(order);

    if (this.useRouterQuery) {
      this.router.navigate([], { queryParams: { order } });
    }
  }
}

export enum SortOrder {
  Downloads = 'download',
  Score = 'score',
}
