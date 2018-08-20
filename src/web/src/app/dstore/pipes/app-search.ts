import { Pipe, PipeTransform } from '@angular/core';

import { App, appSearch } from '../services/app';

@Pipe({
  name: 'appSearch',
})
export class AppSearchPipe implements PipeTransform {
  constructor() {}

  transform(apps: App[], keyword: string): App[] {
    return apps.filter(app => appSearch(app, keyword));
  }
}
