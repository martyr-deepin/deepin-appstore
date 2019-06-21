import { Pipe, PipeTransform } from '@angular/core';
import { range } from 'lodash';
@Pipe({
  name: 'range',
})
export class RangePipe implements PipeTransform {
  transform(value: number, index: number): any {
    return range(value, index);
  }
}
