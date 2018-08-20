import { Pipe, PipeTransform } from '@angular/core';
import { environment } from '../../../environments/environment';
// select image based on ratio
@Pipe({
  name: 'fitImage',
})
export class FitImage implements PipeTransform {
  transform(value: string[] | Array<string>[]) {
    if (value.length === 0) {
      return '';
    }
    const ratio = Math.ceil(devicePixelRatio);
    if (ratio === 1) {
      return value[0] && value[0].length > 0 ? value[0] : value[1];
    } else {
      return value[1] && value[1].length > 0 ? value[1] : value[0];
    }
  }
}
