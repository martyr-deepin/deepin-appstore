import { StoreService } from 'app/modules/client/services/store.service';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { switchMap, map } from 'rxjs/operators';

import { environment } from 'environments/environment';
import { App, AppService } from 'app/services/app.service';
import { JobService } from 'app/services/job.service';
import { StoreJobType } from 'app/modules/client/models/store-job-info';

@Injectable({
  providedIn: 'root',
})
export class RemoteAppService {
  url = environment.operationServer + '/api/user/my/app';
  constructor(
    private http: HttpClient,
    private appService: AppService,
    private storeService: StoreService,
    private jobService: JobService,
  ) {}
  RemoteAppList(page: number, pageSize: number) {
    const params = {
      page: page.toString(),
      count: pageSize.toString(),
    };
    return this.http.get<Result>(this.url, { params }).pipe(
      switchMap(
        result =>
          this.appService.getApps(result.apps.map(app => app.appName), false),
        (result, apps) => {
          result.apps.forEach(remoteApp => {
            remoteApp.app = apps.find(app => app.name === remoteApp.appName);
          });
          return result;
        },
      ),
    );
  }
  installApps(apps: App[]) {
    this.storeService.installPackages(apps);
  }
  installingList() {
    const installType = [StoreJobType.install, StoreJobType.download];
    return this.jobService.jobsInfo().pipe(
      map(jobs => {
        return jobs.reduce(
          (acc, job) => {
            if (!installType.includes(job.type)) {
              return acc;
            }
            return [...acc, ...job.names];
          },
          [] as string[],
        );
      }),
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
  app: App;
}
