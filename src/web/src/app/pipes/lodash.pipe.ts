import { Pipe, PipeTransform } from '@angular/core';
import * as _ from 'lodash';
@Pipe({
  name: 'lodash',
})
export class LodashPipe implements PipeTransform {
  transform(value: any): any {
    return _(value);
  }
}
