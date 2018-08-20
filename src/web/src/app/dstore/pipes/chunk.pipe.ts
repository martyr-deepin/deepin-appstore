import { Pipe, PipeTransform } from '@angular/core';

import { chunk } from 'lodash';

@Pipe({
  name: 'chunk',
})
export class ChunkPipe implements PipeTransform {
  transform<T>(value: Array<T>, size?: number): Array<Array<T>> {
    return chunk(value, size) || [];
  }
}
