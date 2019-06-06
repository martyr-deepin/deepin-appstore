import { Input, Output, EventEmitter } from '@angular/core';
import { Section } from 'app/dstore/services/section';
import { Software } from 'app/services/software.service';
import { environment } from 'environments/environment';

export class SectionItemBase {
  @Input() section: Section;
  @Output() loaded = new EventEmitter<boolean>();
  softs$: Promise<Software[]>;
  constructor() {}

  sortByLocal(zhValue = '', enValue = '') {
    if (environment.locale === 'zh_CN') {
      return -1;
    }
    return 1;
  }
}
