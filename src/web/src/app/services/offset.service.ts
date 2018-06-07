import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root',
})
export class OffsetService {
  private offsetMap = new Map<string, number>();
  constructor() {}
  saveOffset(path: string, offset: number) {
    this.offsetMap.set(path, offset);
  }
  getOffset(path: string) {
    return this.offsetMap.get(path);
  }
}
