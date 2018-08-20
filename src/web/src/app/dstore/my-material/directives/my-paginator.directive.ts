import { Directive, ElementRef } from '@angular/core';
import { MatPaginator } from '@angular/material';

@Directive({
  selector: 'mat-paginator',
})
export class MyPaginatorDirective {
  constructor(paginator: MatPaginator) {
    paginator.initialized.subscribe(() => {
      if (paginator.pageSize === 50) {
        paginator.pageSize = Number(localStorage.getItem('pageSize')) || 8;
      }
      if (paginator.pageSizeOptions.length === 0) {
        paginator.pageSizeOptions = [8, 10, 16];
      }
      paginator.showFirstLastButtons = true;
      paginator.page.subscribe(e => {
        localStorage.setItem('pageSize', e.pageSize);
      });
    });
  }
}
