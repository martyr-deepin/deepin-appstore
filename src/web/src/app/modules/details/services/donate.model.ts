export interface Error {
  code: number;
  content: string;
  key: string;
}
export enum Payment {
  AliPay = 1,
  WeChat,
  PayPal,
}
export interface PayReq {
  appName: string;
  amount: number;
  appStore: string;
  userID?: number;
}
export interface PayResp {
  tradeID: string;
  url: string;
  shortURL: string;
  error: Error;
}

export interface PayCheck {
  isPaying: boolean;
  isExist: boolean;
}
