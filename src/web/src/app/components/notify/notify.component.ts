import { Component, OnInit } from '@angular/core';
import { NotifyService, Notify } from '../../services/notify.service';
import { Observable, of, merge, Subject } from 'rxjs';
import { switchMap, delay } from 'rxjs/operators';

@Component({
  selector: 'app-notify',
  templateUrl: './notify.component.html',
  styleUrls: ['./notify.component.scss'],
})
export class NotifyComponent implements OnInit {
  constructor(private notifyService: NotifyService) {}

  notify$: Observable<Notify>;
  close$ = new Subject<void>();
  ngOnInit() {
    this.notify$ = this.notifyService.obs().pipe(
      switchMap(n => {
        if (!n.delay) {
          return merge(of(n), this.close$);
        } else {
          return merge(of(n), of(undefined).pipe(delay(n.delay)));
        }
      }),
    );
  }
  close() {
    this.close$.next(undefined);
  }
}
