import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root',
})
export class KeyvalueService {
  private store = new Map<string, any>();
  constructor() {}

  add(value: any) {
    const key = Math.random().toString();
    this.store.set(key, value);
    return key;
  }
  get<T = any>(key: string) {
    const value = this.store.get(key);
    return value as T;
  }
}
