export interface Notify {
  type: NotifyType;
  status: NotifyStatus;
  content?: string;
  delay?: number;
}

export enum NotifyType {
  Unknown,
  Reminder,
  Recommend,
  Notice,
}
export enum NotifyStatus {
  Unknown,
  Success,
  Error,
}
