import { MatPaginatorIntl } from '@angular/material';
export class MyMatPaginatorIntl extends MatPaginatorIntl {
  itemsPerPageLabel = '页面大小';
  nextPageLabel = '下一页';
  previousPageLabel = '上一页';

  getRangeLabel = function(page, pageSize, length) {
    return `第${page + 1}/${Math.ceil(length / pageSize)}页`;
  };
}
