import { Pipe, PipeTransform } from '@angular/core';
import { environment } from 'environments/environment';
// select image based on ratio
@Pipe({
  name: 'fitImage',
})
export class FitImage implements PipeTransform {
  transform(value: string[]) {
    if (value.length === 0) {
      return '';
    }
    value = value.map(v => environment.operationServer + '/images/' + v);
    const ratio = Math.ceil(devicePixelRatio);

    if (ratio === 1) {
      return value[0] || value[1];
    } else {
      return value[1] || value[0];
    }
  }
}
