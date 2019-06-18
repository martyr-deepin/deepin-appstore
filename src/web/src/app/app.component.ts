import { Component, OnInit, Injector } from '@angular/core';
import { environment } from 'environments/environment';

import { timeout, first } from 'rxjs/operators';
import { RegionService } from './services/region.service';
import { AuthService, UserInfo } from './services/auth.service';

@Component({
  selector: 'dstore-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent implements OnInit {
  constructor(private inject: Injector) {}
  inited = false;
  ngOnInit() {
    console.log(environment);
    if (environment.autoSelect) {
      this.selectOperation();
    }
  }

  async selectOperation() {
    let region = environment.region;
    let info: UserInfo;
    if (environment.native) {
      const authService = this.inject.get(AuthService);
      info = await authService.info$.pipe(first()).toPromise();
    }
    if (info) {
      region = info.Region;
    } else {
      const regionService = this.inject.get(RegionService);
      try {
        region = await regionService
          .region()
          .pipe()
          .toPromise();
      } catch (err) {
        console.error('region service', err);
      }
    }
    console.log(region);
    if (environment.operationList[region]) {
      environment.operationServer = environment.operationList[region];
    } else {
      environment.operationServer = environment.operationList[environment.region];
    }
    this.inited = true;
  }
}
