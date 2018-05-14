export class StoreJobInfo {
  id = '';
  name = '';
  status = '';
  type = '';
  speed = 0;
  progress = 0.0;
  description = '';
  downloadSize = 0;
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
