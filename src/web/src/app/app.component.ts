import { Component, OnInit, Injector } from '@angular/core';
import { environment } from 'environments/environment';

import { timeout, first, switchMap } from 'rxjs/operators';
import { RegionService } from './services/region.service';
import { AuthService } from './services/auth.service';
import { SettingService } from './services/settings.service';

@Component({
  selector: 'dstore-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent implements OnInit {
  constructor(private inject: Injector) {}
  inited = false;
  ngOnInit() {
    if (environment.autoSelect) {
      this.selectOperation();
    }
  }

  async selectOperation() {
    let region = environment.region;
    const authService = this.inject.get(AuthService);
    const info = await authService.info$.pipe(first()).toPromise();
    if (info) {
      region = info.Region;
    } else {
      const regionService = this.inject.get(RegionService);
      try {
        region = await regionService.region$.pipe(timeout(3000)).toPromise();
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
