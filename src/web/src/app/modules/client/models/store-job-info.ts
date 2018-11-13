export interface StoreJobInfo {
  id: string;
  job: string;
  name: string;
  names: string[];
  status: StoreJobStatus;
  type: StoreJobType;
  speed: number;
  progress: number;
  description: string;
  downloadSize: number;
  createTime: number;
  packages: string;
  cancelable: boolean;
}

export enum StoreJobType {
  download = 'download',
  install = 'install',
  uninstall = 'remove',
}

export enum StoreJobStatus {
  paused = 'paused',
  running = 'running',
  ready = 'ready',
  failed = 'failed',
  end = 'end',
  success = 'success',
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

export interface StoreJobError {
  ErrType: StoreJobErrorType;
  ErrDetail: string;
}
export enum StoreJobErrorType {
  unknown,
  dpkgInterrupted = 'PkgSystemError::dpkgInterrupted',
  dependenciesBroken = 'PkgSystemError::dependenciesBroken',
  invalidSourcesList = 'PkgSystemError::invalidSourcesList',

  removeDDE = 'JobError::removeDDE',
  fetchFailed = 'JobError::fetchFailed',
  dpkgError = 'JobError::dpkgError',
  pkgNotFound = 'JobError::pkgNotFound',
  unmetDependencies = 'JobError::unmetDependencies',
  noInstallationCandidate = 'JobError::noInstallationCandidate',
  insufficientSpace = 'JobError::insufficientSpace',
}
export const CanFixError = [
  StoreJobErrorType.dpkgInterrupted,
  StoreJobErrorType.dependenciesBroken,
];
