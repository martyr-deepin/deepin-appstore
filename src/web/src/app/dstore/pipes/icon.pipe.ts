import { Pipe, PipeTransform } from '@angular/core';
import { BaseService } from '../services/base.service';
import { App } from '../../services/app.service';

@Pipe({
  name: 'icon',
})
export class IconPipe implements PipeTransform {
  server = BaseService.serverHosts.metadataServer;
  transform(app: App): string {
    return this.server + '/' + app.icon;
  }
}
