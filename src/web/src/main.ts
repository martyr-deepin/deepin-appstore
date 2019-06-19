import { enableProdMode, TRANSLATIONS, TRANSLATIONS_FORMAT, MissingTranslationStrategy, NgZone } from '@angular/core';

import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';
import { AppModule } from './app/app.module';
import { environment } from 'environments/environment';

if (environment.production) {
  enableProdMode();
}
// use the require method provided by webpack
declare const require;
//  qt web channel run ng zone
let zone: NgZone = null;

async function main() {
  const QWebChannel = window['QWebChannel'];
  // web mode
  if (!QWebChannel) {
    return bootstrap();
  }
  // Native client mode.
  // js call ==> dstore channel ==> proxy channel >> c++ call

  // proxy channel
  const channelTransport = await new Promise<any>(resolve => {
    return new QWebChannel(window['qt'].webChannelTransport, resolve);
  });
  // dstore channel
  const channel = await new Promise<any>(resolve => {
    const t = {
      send(msg: any) {
        channelTransport.objects.channelProxy.send(msg);
      },
      onmessage(msg: any) {},
    };
    channelTransport.objects.channelProxy.message.connect(msg => {
      if (!zone) {
        t.onmessage({ data: msg });
      } else {
        zone.run(() => {
          t.onmessage({ data: msg });
        });
      }
    });
    return new QWebChannel(t, resolve);
  });

  window['dstore'] = { channel };

  const settings = await new Promise<Settings>(resolve => {
    channel.objects.settings.getSettings(resolve);
  });
  console.log('dstore client config', settings);
  environment.native = true;
  environment.themeName = settings.themeName;
  if (environment.production) {
    environment.supportSignIn = settings.supportSignIn;
    environment.region = settings.defaultRegion;
    environment.autoSelect = settings.allowSwitchRegion;
    environment.operationList = settings.operationServerMap;
    environment.metadataServer = settings.metadataServer;
    environment.operationServer = environment.operationList[environment.region];
  }
  environment.autoSelect = false;
  environment.region = 'CN';
  environment.operationServer = environment.operationList[environment.region];

  // if (!Boolean(settings['aot'])) {
  // loading i18n files
  for (let language of navigator.languages) {
    language = language.replace('-', '_');
    try {
      const translations = require(`raw-loader!./locale/messages.${language}.xlf`);
      if (translations) {
        return bootstrap(translations);
      }
    } catch (err) {
      console.error('cannot load locale', language, err);
    }
  }
  return bootstrap();
}

function bootstrap(translations = null) {
  let opt = {};
  if (translations) {
    opt = {
      missingTranslation: MissingTranslationStrategy.Warning,
      providers: [{ provide: TRANSLATIONS, useValue: translations }, { provide: TRANSLATIONS_FORMAT, useValue: 'xlf' }],
    };
  }
  console.log(opt);
  return platformBrowserDynamic().bootstrapModule(AppModule, opt);
}

main()
  .catch(err => {
    console.error('load locale fail', err);
    return bootstrap();
  })
  .then(app => {
    window['app'] = app;
    zone = app.injector.get(NgZone);
  })
  .finally(() => {
    console.log('bootstrap');
  });

interface Settings {
  allowShowPackageName: boolean;
  allowSwitchRegion: boolean;
  autoInstall: boolean;
  defaultRegion: string;
  metadataServer: string;
  operationServerMap: { [key: string]: string };
  remoteDebug: boolean;
  supportAot: boolean;
  supportSignIn: boolean;
  themeName: string;
  upyunBannerVisible: boolean;
}
