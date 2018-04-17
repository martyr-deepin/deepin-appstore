import { Injectable } from '@angular/core';
import { AppService } from './app.service';

@Injectable()
export class StoreDaemonService {

  constructor(private appService: AppService) {
  }

  /**
   * Send app list to backend.
   */
  saveAppList() {
    this.appService.list.subscribe(resp => {
      console.log('dstore daemon app list: ', resp);
    })
    ;
  }
}
