import { Pipe, PipeTransform } from '@angular/core';
import { environment } from '../../../environments/environment';
// 在数组里选择合适的值，用于选择多语言
@Pipe({
  name: 'fitLanguage',
})
export class FitLanguage implements PipeTransform {
  transform(value: string[]): string {
    if (navigator.language === 'zh-CN') {
      return value[0] || value[1] || '';
    } else {
      return value[1] || value[0] || '';
    }
  }
}
