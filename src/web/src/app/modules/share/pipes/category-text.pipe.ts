import { Pipe, PipeTransform } from '@angular/core';
import { CategoryService } from 'app/services/category.service';

@Pipe({
  name: 'categoryText',
})
export class CategoryTextPipe implements PipeTransform {
  constructor(private category: CategoryService) {}
  categoryMap$ = this.category.metadataCategory$;
  transform(category: string) {
    return this.categoryMap$.then(map => map.get(category));
  }
}
