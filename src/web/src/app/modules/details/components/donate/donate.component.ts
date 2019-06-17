import { Component, OnInit, Input, Output, EventEmitter, ChangeDetectorRef, NgZone, ViewChild } from '@angular/core';
import { DomSanitizer, SafeResourceUrl } from '@angular/platform-browser';

import { Observable, of, iif, timer } from 'rxjs';
import { map, catchError, switchMap, tap, find, takeWhile, pairwise } from 'rxjs/operators';
import { debounce } from 'lodash';

import * as QRCode from 'qrcode';

import { Payment, PayReq, PayCheck } from '../../services/donate.model';
import { DonateService } from '../../services/donate.service';
import { AuthService } from 'app/services/auth.service';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { DonorsComponent } from '../donors/donors.component';
import { environment } from 'environments/environment';

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
  @ViewChild(DonorsComponent, { static: true })
  donors: DonorsComponent;
  @Input()
  appName: string;
  amount = 9.9;
  Payment = Payment;
  payment: Payment = Payment.WeChat;
  randAmount = [2.0, 5.2, 8.88, 6.66, 9.9, 18.0, 12.0, 66.0, 25.5, 9.99, 15.2];
  loading: boolean;
  qrImg: SafeResourceUrl;
  waitPay$: Observable<PayCheck>;
  done: boolean;

  ngOnInit() {}

  close() {
    this.amount = 9.9;
    this.waitPay$ = null;
    this.payment = Payment.WeChat;
    this.loading = false;
    this.qrImg = null;
  }

  rand() {
    let r = this.amount;
    while (r === this.amount) {
      r = this.randAmount[Math.floor(Math.random() * this.randAmount.length)];
    }
    this.amount = r;
  }

  pay() {
    console.log('start');
    this.loading = true;
    this.authService.info$
      .pipe(
        map(info => {
          console.log('get');
          const req: PayReq = {
            appStore: environment.region === 'Default' ? 'international' : 'china',
            appName: this.appName,
            amount: this.amount * 100,
          };
          if (info) {
            req.userID = info.UserID;
          }
          return req;
        }),
        switchMap(
          req => this.donateService.donate(this.payment, req),
          (req, resp) => {
            return { req, resp };
          },
        ),
      )
      .subscribe(({ req, resp }) => {
        if (resp.error) {
          console.error(resp);
          return;
        }
        if (this.payment === Payment.WeChat) {
          if (!resp.shortURL) {
            return;
          }
          QRCode.toDataURL(resp.shortURL).then(
            url => (this.qrImg = this.sanitizer.bypassSecurityTrustResourceUrl(url)),
          );
        } else {
          DstoreObject.openURL(resp.url);
        }
        this.loading = false;
        this.waitPay$ = timer(0, 2000).pipe(
          switchMap(() => this.donateService.check(this.payment, resp.tradeID)),
          pairwise(),
          takeWhile(result => !result[0].isExist),
          map(result => result[1]),
          tap(c => {
            if (c.isExist) {
              DstoreObject.raiseWindow();
              this.donors.add(req.userID);
            }
          }),
        );
      });
  }

  inputChange(e: Event) {
    const el = e.target as HTMLInputElement;
    if (!el.value.match(/^\d{0,6}(\.\d{0,2})?$/)) {
      el.value = this.amount.toString();
    }
    if (el.value) {
      this.amount = parseFloat(el.value);
    } else {
      this.amount = null;
    }
  }
}
