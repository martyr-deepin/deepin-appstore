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
    if (devicePixelRatio > 1) {
      value.reverse();
    }
    return value.filter(Boolean).map(v => environment.operationServer + '/images/' + v)[0];
  }
}
