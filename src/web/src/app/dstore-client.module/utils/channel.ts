import * as _ from 'lodash';
import { Observable } from 'rxjs';
export class Channel {
  static getSlot(path: string): Function {
    return _.get(window, 'dstore.channel.objects.' + path);
  }
  static getSignal(path: string) {
    interface Signal {
      connect: (callback: Function) => void;
      disconnect: (callback: Function) => void;
    }
    return _.get(window, 'dstore.channel.objects.' + path) as Signal;
  }

  static exec<T>(method: string, ...args: any[]): Promise<T> {
    console.warn('exec', method, args);
    return new Promise<T>(resolve => {
      Channel.getSlot(method)(...args, resolve);
    });
  }
  static connect<T>(method: string): Observable<T> {
    console.warn('connect', method);
    return new Observable<T>(obs => {
      const callback = (...resp) => {
        obs.next(...resp);
      };
      Channel.getSignal(method).connect(callback);
      return () => {
        Channel.getSignal(method).disconnect(callback);
      };
    });
  }
}
