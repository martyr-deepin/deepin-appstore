import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import { DomSanitizer, SafeResourceUrl } from '@angular/platform-browser';

import { Observable, of, iif, timer } from 'rxjs';
import { map, catchError, switchMap, tap } from 'rxjs/operators';

import * as QRCode from 'qrcode';

import { UserService } from '../../services/user.service';
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
    private userService: UserService,
    private donateService: DonateService,
    private sanitizer: DomSanitizer,
  ) {}
  @Input() appName: string;
  @Output() close = new EventEmitter<void>();
  amount = 2;
  Payment = Payment;
  payment: Payment = Payment.WeChat;
  randAmount = [2.0, 5.2, 8.88, 6.66, 18.0, 12.0, 66.0, 25.5, 9.99, 15.2];
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
    iif(
      () => this.authService.isLoggedIn,
      this.userService.myInfo().pipe(
        map(info => {
          return {
            appName: this.appName,
            amount: this.amount * 100,
            userID: info.uid,
          };
        }),
      ),
      of({
        appName: this.appName,
        amount: this.amount * 100,
      }),
    )
      .pipe(switchMap(req => this.donateService.donate(this.payment, req)))
      .subscribe(resp => {
        if (this.payment === Payment.WeChat) {
          QRCode.toDataURL(resp.url).then(
            url => (this.qrImg = this.sanitizer.bypassSecurityTrustResourceUrl(url)),
          );
        } else {
          DstoreObject.openURL(resp.url);
        }
        this.waitPay$ = timer(0, 1000).pipe(
          switchMap(() => this.donateService.check(resp.tradeID)),
          tap(c => {
            if (c.isExist) {
              // this.close.emit();
            }
          }),
        );
      });
  }
  inputChange(el: HTMLInputElement) {
    this.amount = Math.floor(parseFloat(el.value) * 100) / 100;
    el.value = this.amount.toString();
  }
}
