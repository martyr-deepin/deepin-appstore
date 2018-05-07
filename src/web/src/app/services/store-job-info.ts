export class StoreJobInfo {
  id = '';
  name = '';
  status = '';
  type = '';
  speed = 0;
  progress = 0.0;
  description = '';
  packages: string[] = [];
  cancelable = false;
}

export enum AppJobStatus {
  undefined,
  ready,
  running,
  finish,
}
export interface AppJobInfo {
  status: AppJobStatus;
  jobInfo: StoreJobInfo;
}
