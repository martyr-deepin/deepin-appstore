import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import { DomSanitizer, SafeResourceUrl } from '@angular/platform-browser';

import { Observable, of, iif, timer } from 'rxjs';
import { map, catchError, switchMap, tap } from 'rxjs/operators';
import { debounce } from 'lodash';

import * as QRCode from 'qrcode';

import { Payment, PayReq, PayCheck } from '../../services/donate.model';
import { DonateService } from '../../services/donate.service';
import { AuthService } from '../../services/auth.service';
import { DstoreObject } from '../../dstore-client.module/utils/dstore-objects';

@Component({
  selector: 'app-donate',
  templateUrl: './donate.component.html',
  styleUrls: ['./donate.component.scss'],
})
export class DonateComponent implements OnInit {
  constructor(
    private authService: AuthService,
    private donateService: DonateService,
    private sanitizer: DomSanitizer,
  ) {}
  @Input() appName: string;
  @Output() close = new EventEmitter<void>();
  amount = 2;
  Payment = Payment;
  payment: Payment = Payment.WeChat;
  randAmount = [2.0, 5.2, 8.88, 6.66, 18.0, 12.0, 66.0, 25.5, 9.99, 15.2];
  loading: boolean;
  qrImg: SafeResourceUrl;
  waitPay$: Observable<PayCheck>;

  ngOnInit() {}

  rand() {
    let r = this.amount;
    while (r === this.amount) {
      r = this.randAmount[Math.floor(Math.random() * this.randAmount.length)];
    }
    this.amount = r;
  }

  pay() {
    this.loading = true;
    this.authService.info$
      .pipe(
        map(info => {
          const req: PayReq = {
            appName: this.appName,
            amount: this.amount * 100,
          };
          if (info) {
            req.userID = info.userID;
          }
          return req;
        }),
        switchMap(req => this.donateService.donate(this.payment, req)),
      )
      .subscribe(resp => {
        if (resp.error || !resp.shortURL) {
          console.error(resp);
          return;
        }
        if (this.payment === Payment.WeChat) {
          QRCode.toDataURL(resp.shortURL).then(
            url => (this.qrImg = this.sanitizer.bypassSecurityTrustResourceUrl(url)),
          );
        } else {
          DstoreObject.openURL(resp.url);
        }
        this.loading = false;
        this.waitPay$ = timer(0, 1000).pipe(
          switchMap(() => this.donateService.check(this.payment, resp.tradeID)),
          tap(c => {
            if (c.isExist) {
              // this.close.emit();
            }
          }),
        );
      });
  }

  inputChange(e: Event) {
    const el = e.target as HTMLInputElement;
    if (!el.value.match(/^\d{0,9}(\.\d{0,2})?$/)) {
      el.value = this.amount.toString();
    }
    if (el.value) {
      this.amount = parseFloat(el.value);
    } else {
      this.amount = null;
    }
  }
}
