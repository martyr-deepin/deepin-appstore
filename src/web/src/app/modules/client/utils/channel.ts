import * as _ from 'lodash';
import { Observable } from 'rxjs';
export class Channel {
  static getSlot(path: string): Function {
    const emptySlot = () => null;
    return _.get(window, 'dstore.channel.objects.' + path, emptySlot);
  }
  static getSignal(path: string) {
    interface Signal {
      connect: (callback: Function) => void;
      disconnect: (callback: Function) => void;
    }
    const emptySignal: Signal = {
      connect(callback) {},
      disconnect(callback) {},
    };
    return _.get(window, 'dstore.channel.objects.' + path, emptySignal) as Signal;
  }

  static exec<T>(method: string, ...args: any[]): Promise<T> {
    return new Promise<T>(resolve => {
      Channel.getSlot(method)(...args, resp => {
        resolve(resp);
        console.warn('exec', method, args, resp);
      });
    });
  }
  static connect<T>(method: string): Observable<T> {
    console.warn('connect', method);
    return new Observable<T>(obs => {
      const callback = (...resp) => {
        if (resp.length > 1) {
          obs.next(resp as any);
        } else {
          obs.next(resp[0]);
        }
      };
      Channel.getSignal(method).connect(callback);
      return () => {
        Channel.getSignal(method).disconnect(callback);
      };
    });
  }
}
