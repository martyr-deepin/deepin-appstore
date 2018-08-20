import { Pipe, PipeTransform } from '@angular/core';
import { environment } from '../../../environments/environment';
// output of only debug
@Pipe({
  name: 'dev',
})
export class DevPipe implements PipeTransform {
  transform(value: any): '' {
    if (environment.production) {
      return null;
    }
    return value;
  }
}
