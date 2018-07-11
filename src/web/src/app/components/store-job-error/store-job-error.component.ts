import { Component, OnInit, Input } from '@angular/core';
import { StoreJobErrorType, StoreJobError } from '../../dstore-client.module/models/store-job-info';

@Component({
  selector: 'app-store-job-error',
  templateUrl: './store-job-error.component.html',
  styleUrls: ['./store-job-error.component.scss'],
})
export class StoreJobErrorComponent implements OnInit {
  constructor() {}

  StoreJobErrorType = StoreJobErrorType;

  @Input()
  set rowString(s: string) {
    try {
      const err = JSON.parse(s) as StoreJobError;
      this.errType = err.ErrType;
      this.errDetail = err.ErrDetail;
    } catch {
      this.errType = StoreJobErrorType.unknown;
      this.errDetail = s;
    }
  }
  errType: StoreJobErrorType;
  errDetail: string;
  ngOnInit() {}
  click(event: Event) {
    event.stopPropagation();
  }
}
