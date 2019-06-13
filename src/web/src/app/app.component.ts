import { Component, OnInit } from '@angular/core';
import { environment } from 'environments/environment';

import { timeout } from 'rxjs/operators';
import { RegionService } from './services/region.service';

@Component({
  selector: 'dstore-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent implements OnInit {
  constructor(private regionService: RegionService) {}
  inited = false;
  ngOnInit() {
    if (environment.autoSelect) {
      this.selectOperation();
    }
  }

  async selectOperation() {
    this.regionService.region$.pipe(timeout(3000)).subscribe(
      region => {
        if (region.Country.IsoCode === 'CN') {
          environment.operationServer = environment.operationList.primary;
        } else {
          environment.operationServer = environment.operationList.secondary;
        }
      },
      err => console.error(err),
      () => (this.inited = true),
    );
  }
}
