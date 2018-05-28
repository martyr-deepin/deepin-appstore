import { Error } from '../dstore/services/errno';

export enum Payment {
  WeiChatPay = 1,
  AliPay,
  PayPal,
}
export interface PayReq {
  appName: string;
  amount: number;
  userID?: number;
}
export interface PayResp {
  tradeID: string;
  url: string;
  error: Error;
}

export interface PayCheck {
  isExist: boolean;
}
