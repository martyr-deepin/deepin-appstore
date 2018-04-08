import { enableProdMode } from '@angular/core';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

import { AppModule } from './app/app.module';
import { environment } from './environments/environment';

if (environment.production) {
  enableProdMode();
}

const bootstrap = () => {
  platformBrowserDynamic().bootstrapModule(AppModule)
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
