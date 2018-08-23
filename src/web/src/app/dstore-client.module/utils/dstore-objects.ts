import { result, get, noop } from 'lodash';
import { Observable } from 'rxjs';

const DstoreObjectPath = 'dstore.channel.objects'.split('.');

export class DstoreObject {
  static openURL(url: string): void {
    console.log('openURL', url);
    get(window, [...DstoreObjectPath, 'settings', 'openUrl'], noop)(url);
  }

  static raiseWindow() {
    console.log('raiseWindow');
    result(window, [...DstoreObjectPath, 'settings', 'raiseWindow']);
  }

  static getServers(): Promise<Servers> {
    return new Promise<Servers>((resolve, reject) => {
      get(window, [...DstoreObjectPath, 'settings', 'getServers'], noop)((s: Servers) =>
        resolve(s),
      );
    });
  }

  static AdVisible(): Promise<boolean> {
    return new Promise<boolean>((resolve, reject) => {
      get(window, [...DstoreObjectPath, 'settings', 'upyunBannerVisible'], noop)((show: boolean) =>
        resolve(show),
      );
    });
  }

  static imageViewer(src: string, data: string) {
    get(window, [...DstoreObjectPath, 'imageViewer', 'openBase64'], noop)(src, data);
  }
  static imagesPreview(src: string[], index: number) {
    get(window, [...DstoreObjectPath, 'imageViewer', 'setImageList'], noop)(src, index);
  }
  static openOnlineImage(): Observable<string> {
    return new Observable<string>(obs => {
      console.log('openOnlineImage');
      const openOnlineImage: Signal = get(window, [
        ...DstoreObjectPath,
        'imageViewer',
        'openOnlineImageRequest',
      ]);
      console.log(openOnlineImage, [...DstoreObjectPath, 'imageViewer', 'openOnlineImageRequest']);
      if (openOnlineImage) {
        const callback = (src: string) => obs.next(src);
        openOnlineImage.connect(callback);
        return () => openOnlineImage.disconnect(callback);
      }
    });
  }

  static clearArchives(): Observable<void> {
    return new Observable<void>(obs => {
      const clearArchives: Signal = get(window, [
        ...DstoreObjectPath,
        'storeDaemon',
        'clearArchives',
      ]);
      if (!clearArchives) {
        obs.error(new Error('do not get'));
      }
      const callBack = () => {
        obs.next();
      };
      clearArchives.connect(callBack);
      return () => {
        clearArchives.disconnect(callBack);
      };
    });
  }
}

interface Servers {
  metadataServer: string;
  operationServer: string;
}
interface Signal {
  connect: (CallBack) => {};
  disconnect: (CallBack) => {};
}

type CallBack = (result?: any[]) => {};
