import { enableProdMode, MissingTranslationStrategy, TRANSLATIONS, TRANSLATIONS_FORMAT } from '@angular/core';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

import { AppModule } from './app/app.module';
import { environment } from './environments/environment';
import { Locale } from './app/utils/locale';

if (environment.production) {
  enableProdMode();
}

declare const require;

const bootstrap = () => {
  const locale = Locale.getAngularLocale();
  const bootstrapModuleAlias = platformBrowserDynamic().bootstrapModule;
  let compilerOptions: object[];
  if (Locale.localeFileExists(locale)) {
    const translations = require(`raw-loader!./locale/messages.${locale}.xlf`);
    const compilerOption = {
      missingTranslation: MissingTranslationStrategy.Ignore,
      providers: [
        {
          provide: TRANSLATIONS,
          useValue: translations
        },
        {
          provide: TRANSLATIONS_FORMAT,
          useValue: 'xlf'
        }
      ]
    };
    compilerOptions = [compilerOption];
  } else {
    compilerOptions = [];
  }
  platformBrowserDynamic().bootstrapModule(AppModule, ...compilerOptions)
    .catch(err => console.log(err));
};

if (window['QWebChannel'] !== undefined) {
  // Native client mode.
  // noinspection TsLint
  new window['QWebChannel'](window['qt'].webChannelTransport, (channel) => {
    window['channel'] = channel;
    bootstrap();
  });
} else {
  // Browser mode.
  bootstrap();
}
