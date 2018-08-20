import { Injectable, NgZone } from '@angular/core';

import { Subject } from 'rxjs/Subject';
import 'rxjs/add/operator/throttleTime';

declare var Materialize: any;

@Injectable()
export class MaterializeService {
  private toastSubject = new Subject<ToastMessage>();

  constructor(private zone: NgZone) {
    this.toastSubject.throttleTime(500).subscribe(toastInfo => {
      if (Materialize && Materialize.toast) {
        this.zone.runOutsideAngular(() => {
          Materialize.toast(toastInfo.body, toastInfo.delay);
        });
      }
    });
  }

  toastSuccess(msg: string) {
    this.toastSubject.next({
      body: `<span class='green-text'>${msg}</span>`,
      delay: 2000,
    });
  }

  toastError(msg: string) {
    this.toastSubject.next({
      body: `<span class='red-text'>${msg}</span>`,
      delay: 3000,
    });
  }
  updateTextFields() {
    if (Materialize && Materialize.updateTextFields) {
      Materialize.updateTextFields();
    }
  }
}
interface ToastMessage {
  body: string;
  delay: number;
}
