import {
  enableProdMode,
  TRANSLATIONS,
  TRANSLATIONS_FORMAT,
  MissingTranslationStrategy,
} from '@angular/core';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

import { AppModule } from './app/app.module';
import { environment } from './environments/environment';
import { Locale } from './app/dstore/utils/locale';

if (environment.production) {
  enableProdMode();
}

declare const require;

const loadTranslations = new Promise((resolve, reject) => {
  const lang = Locale.getUnixLocale();
  if (lang === 'en-US') {
    resolve(null);
  }
  const translations = require(`raw-loader!./locale/messages.${lang}.xlf`);
  resolve(translations);
});

const bootstrap = () => {
  console.log('bootstrap');
  loadTranslations
    .catch(err => {
      console.error('loadTranslations error:', err);
      return null;
    })
    .then(translations => {
      platformBrowserDynamic().bootstrapModule(AppModule, {
        missingTranslation: MissingTranslationStrategy.Warning,
        providers: [
          { provide: TRANSLATIONS, useValue: translations },
          { provide: TRANSLATIONS_FORMAT, useValue: 'xlf' },
        ],
      });
    })
    .catch(err => {
      console.log('translations error:', err);
      platformBrowserDynamic().bootstrapModule(AppModule);
    })
    .catch(console.log);
};

if (window['QWebChannel'] !== undefined) {
  // Native client mode.
  // noinspection TsLint
  // tslint:disable-next-line:no-unused-expression
  new window['QWebChannel'](window['qt'].webChannelTransport, channel => {
    window['dstore'] = {};
    window['dstore']['channel'] = channel;
    channel.objects.settings.getServers((obj: Object) => {
      // These properties are defined in src/ui/channel/settings_proxy.cpp
      window['dstore']['metadataServer'] = obj['metadataServer'];
      window['dstore']['operationServer'] = obj['operationServer'];

      bootstrap();
    });
  });
} else {
  // Browser mode.
  bootstrap();
}
