import { Pipe, PipeTransform } from '@angular/core';
import { Observable } from 'rxjs';
import { shareReplay } from 'rxjs/operators';

import { memoize } from 'lodash';

import { AppService } from '../services/app.service';

@Pipe({
  name: 'appInfo',
})
export class AppInfoPipe implements PipeTransform {
  constructor(private appService: AppService) {}

  transform = memoize(
    name => (!name ? undefined : this.appService.getAppByName(name).pipe(shareReplay())),
  );
}
