import { environment } from 'environments/environment';
import { has, get } from 'lodash';
export class BaseService {
  constructor() {}

  static get serverHosts(): ServerHosts {
    return {
      operationServer: environment.operationServer,
      metadataServer: environment.metadataServer,
    };
  }

  static get domainName() {
    return this.serverHosts.operationServer.split(/[-\.]/)[2];
  }

  static get isNative(): boolean {
    return has(window, ['dstore', 'channel']);
  }

  static get whiteList(): string[] {
    const list = [this.serverHosts.operationServer].map((url: string) => {
      return url.slice(url.indexOf('://') + 3);
    });
    return list;
  }
}

interface ServerHosts {
  operationServer: string;
  metadataServer: string;
}
