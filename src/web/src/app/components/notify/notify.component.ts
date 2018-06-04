import { Component, OnInit } from '@angular/core';
import { NotifyService } from '../../services/notify.service';
import { Notify, NotifyType, NotifyStatus } from '../../services/notify.model';
import { Observable, of, merge, Subject, concat } from 'rxjs';
import { switchMap, delay, tap } from 'rxjs/operators';

@Component({
  selector: 'app-notify',
  templateUrl: './notify.component.html',
  styleUrls: ['./notify.component.scss'],
})
export class NotifyComponent implements OnInit {
  constructor(private notifyService: NotifyService) {}
  NotifyType = NotifyType;
  NotifyStatus = NotifyStatus;

  notify$: Observable<Notify>;
  close$ = new Subject<void>();
  ngOnInit() {
    this.notify$ = this.notifyService.obs().pipe(
      switchMap(n => {
        console.log(n);
        if (!n.delay) {
          return concat(of(n), this.close$);
        } else {
          return concat(of(n), of(undefined).pipe(delay(n.delay)));
        }
      }),
    );
  }
  close() {
    this.close$.next(undefined);
  }
}
