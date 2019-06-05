import {
  enableProdMode,
  TRANSLATIONS,
  TRANSLATIONS_FORMAT,
  MissingTranslationStrategy,
  CompilerOptions,
  NgZone,
} from '@angular/core';

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
  let opts: CompilerOptions;
  // Native client mode.
  const QWebChannel = window['QWebChannel'];
  if (QWebChannel) {
    // proxy channel
    const channelTransport = await new Promise<any>(resolve => {
      return new QWebChannel(window['qt'].webChannelTransport, resolve);
    });
    // dstore channel
    const channel = await new Promise<any>(resolve => {
      const t = {
        send(msg) {
          channelTransport.objects.channelProxy.send(msg);
        },
        onmessage(msg) {},
      };
      channelTransport.objects.channelProxy.message.connect(msg => {
        if (zone) {
          zone.run(() => {
            t.onmessage({ data: msg });
          });
          return;
        }
        t.onmessage({ data: msg });
      });
      return new QWebChannel(t, resolve);
    });

    window['dstore'] = { channel };

    const servers = await new Promise(resolve => {
      channel.objects.settings.getServers(resolve);
    });
    environment.supportSignIn = servers['supportSignIn'];
    if (environment.production) {
      environment.metadataServer = servers['metadataServer'];
      environment.operationServer = servers['operationServer'];
    }
    environment.themeName = servers['themeName'];

    if (!Boolean(servers['aot'])) {
      // loading locale file
      for (let language of navigator.languages) {
        language = language.replace('-', '_');
        try {
          const translations = require(`raw-loader!./locale/messages.${language}.xlf`);
          if (translations != null) {
            opts = {
              missingTranslation: MissingTranslationStrategy.Warning,
              providers: [
                { provide: TRANSLATIONS, useValue: translations },
                { provide: TRANSLATIONS_FORMAT, useValue: 'xlf' },
              ],
            };
          }
          break;
        } catch (err) {
          console.error('cannot load locale', language, err);
        }
      }
    }
  }
  return bootstrap(opts);
}
function bootstrap(opts = null) {
  return platformBrowserDynamic().bootstrapModule(AppModule, opts);
}
main()
  .catch(err => {
    console.error('load locale fail', err);
    return bootstrap();
  })
  .then(app => {
    zone = app.injector.get(NgZone);
  })
  .finally(() => {
    console.log('bootstrap');
  });
