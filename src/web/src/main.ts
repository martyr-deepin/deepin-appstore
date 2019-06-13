import { enableProdMode, TRANSLATIONS, TRANSLATIONS_FORMAT, MissingTranslationStrategy, NgZone } from '@angular/core';

import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';
import { AppModule } from './app/app.module';
import { environment } from './environments/environment';

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

  const servers = await new Promise(resolve => {
    channel.objects.settings.getServers(resolve);
  });
  console.log('client config', servers);
  environment.native = true;
  environment.themeName = servers['themeName'];
  if (environment.production) {
    environment.supportSignIn = servers['supportSignIn'];
    environment.metadataServer = servers['metadataServer'];
    environment.operationServer = servers['operationServer'];
  }

  if (!Boolean(servers['aot'])) {
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
  }
}

function bootstrap(translations = null) {
  if (translations) {
    return platformBrowserDynamic().bootstrapModule(AppModule, {
      missingTranslation: MissingTranslationStrategy.Warning,
      providers: [{ provide: TRANSLATIONS, useValue: translations }, { provide: TRANSLATIONS_FORMAT, useValue: 'xlf' }],
    });
  }
  return platformBrowserDynamic().bootstrapModule(AppModule);
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
