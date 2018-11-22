import { Injectable } from '@angular/core';
import { switchMap, map } from 'rxjs/operators';

import { JobService } from 'app/services/job.service';
import { AppService, App } from 'app/services/app.service';
import { StoreService } from 'app/modules/client/services/store.service';
import { InstalledApp } from 'app/modules/client/models/installed';
import { StoreJobType } from 'app/modules/client/models/store-job-info';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class LocalAppService {
  metadataServer = environment.metadataServer;
  constructor(
    private jobService: JobService,
    private storeService: StoreService,
    private appService: AppService,
  ) {}
  LocalAppList() {
    return this.jobService.jobList().pipe(
      switchMap(() => this.storeService.getInstalledApps()),
      switchMap(
        installed => this.appService.getApps(installed.map(app => app.name)),
        (installed, appInfos) => {
          return installed
            .map(app => {
              return this.appService.addApp(
                app,
                appInfos.find(info => info.packageURI.includes(app.dpk)),
              );
            })
            .filter(app => Boolean(app.app))
            .sort((a, b) => b.time - a.time);
        },
      ),
    );
  }
  RemovingList() {
    return this.jobService.jobsInfo().pipe(
      map(jobs => {
        return jobs
          .filter(job => job.type === StoreJobType.uninstall)
          .map(job => job.names)
          .reduce((acc, names) => {
            acc.push(...names);
            return acc;
          }, []);
      }),
    );
  }
  RemoveLocalApp(app: App) {
    this.storeService.removePackage(app.name, app.localInfo.description.name);
  }
}
