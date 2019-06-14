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
    const authService = this.inject.get(AuthService);
    authService.info$
      .pipe(
        first(),
        switchMap(async info => {
          if (info) {
            return info.Region;
          }
          const regionService = this.inject.get(RegionService);
          return regionService.region$.pipe(timeout(3000)).toPromise();
        }),
      )
      .subscribe(
        region => {
          console.log('switch region', region, environment.operationList[region]);
          if (environment.operationList[region]) {
            environment.operationServer = environment.operationList[region];
          } else {
            environment.operationServer = environment.operationList[environment.region];
          }
        },
        err => console.error(err),
        () => (this.inited = true),
      );
  }
}
