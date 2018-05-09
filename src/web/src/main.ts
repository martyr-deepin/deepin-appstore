import { enableProdMode } from '@angular/core';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

import { AppModule } from './app/app.module';
import { environment } from './environments/environment';

if (environment.production) {
  enableProdMode();
}

declare const require;

const bootstrap = () => {
  platformBrowserDynamic()
    .bootstrapModule(AppModule)
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
