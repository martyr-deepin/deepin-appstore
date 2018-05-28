import { Injectable } from '@angular/core';
import { Payment, PayReq, PayResp, PayCheck } from './donate.model';
import { BaseService } from '../dstore/services/base.service';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root',
})
export class DonateService {
  server = BaseService.serverHosts.metadataServer;
  constructor(private http: HttpClient) {}
  donate(payment: Payment, req: PayReq) {
    switch (payment) {
      case Payment.WeiChatPay:
        return this.http.post<PayResp>(this.server + '/api/donate/wechatpay', req);
      case Payment.AliPay:
        return this.http.post<PayResp>(this.server + '/api/donate/alipay', req);
      case Payment.PayPal:
        return this.http.post<PayResp>(this.server + '/api/donate/paypal', req);
    }
  }
  check(id: string) {
    return this.http.get<PayCheck>(this.server + '/api/donate/check/' + id);
  }
}
