import { Channel } from 'app/modules/client/utils/channel';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root',
})
export class SysFontService {
  constructor() {}
  get fontChange$() {
    return Channel.connect<[string, number]>('settings.fontChangeRequested');
  }
}
