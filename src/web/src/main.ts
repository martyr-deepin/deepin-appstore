import {
  enableProdMode,
  MissingTranslationStrategy,
  TRANSLATIONS,
  TRANSLATIONS_FORMAT
} from '@angular/core';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

import { AppModule } from './app/app.module';
import { environment } from './environments/environment';
import { Locale } from './app/utils/locale';

if (environment.production) {
  enableProdMode();
}

declare const require;

const bootstrap = () => {
  const bootstrapModuleAlias = platformBrowserDynamic().bootstrapModule;
  let compilerOptions: object[];
  if (Locale.localeFileExists()) {
    const translations = Locale.getUnixLocaleFileContent();
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
  platformBrowserDynamic()
    .bootstrapModule(AppModule, ...compilerOptions)
    .catch(err => console.log(err));
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
