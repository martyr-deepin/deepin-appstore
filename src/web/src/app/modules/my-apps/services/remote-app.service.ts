import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { switchMap, map, distinctUntilChanged } from 'rxjs/operators';

import { environment } from 'environments/environment';
import { JobService } from 'app/services/job.service';
import { StoreJobType } from 'app/modules/client/models/store-job-info';
import { Software, SoftwareService } from 'app/services/software.service';

@Injectable({
  providedIn: 'root',
})
export class RemoteAppService {
  url = environment.operationServer + '/api/user/my/app';
  constructor(private http: HttpClient, private softService: SoftwareService, private jobService: JobService) {}
  RemoteAppList(page: number, pageSize: number) {
    const params = {
      page: page.toString(),
      count: pageSize.toString(),
    };
    return this.jobService.jobList().pipe(
      map(list => list.length),
      distinctUntilChanged(),
      switchMap(() => this.http.get<Result>(this.url, { params })),
      switchMap(async result => {
        const list = await this.softService.list({
          names: result.apps.map(app => app.appName),
          filterPackage: false,
          filterStat: false,
        });
        const softMap = new Map(list.map(soft => [soft.name, soft]));
        return {
          total: result.totalCount,
          apps: result.apps
            .filter(app => softMap.has(app.appName))
            .map(app => {
              app.app = softMap.get(app.appName);
              return app;
            }),
        };
      }),
    );
  }

  installApps(softs: Software[]) {
    this.softService.install(...softs);
  }

  installingList() {
    const installType = [StoreJobType.install, StoreJobType.download];
    return this.jobService
      .jobsInfo()
      .pipe(
        map(jobs =>
          ([] as string[]).concat(...jobs.filter(job => installType.includes(job.type)).map(job => job.names)),
        ),
      );
  }
}

interface Result {
  apps: RemoteApp[];
  totalCount: number;
}
interface RemoteApp {
  appName: string;
  time: Date;
  app: Software;
}
