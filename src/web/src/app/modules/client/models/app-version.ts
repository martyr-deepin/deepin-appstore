export interface AppVersion {
  name: string;
  packageURI: string;
  localVersion: string;
  remoteVersion: string;
  upgradable: boolean;
  packageSize: number;
  downloadSize: number;
  installedTime: number;
}
