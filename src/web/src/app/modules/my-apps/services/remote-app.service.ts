import { StoreService } from 'app/modules/client/services/store.service';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { switchMap, map } from 'rxjs/operators';

import { environment } from 'environments/environment';
import { JobService } from 'app/services/job.service';
import { StoreJobType } from 'app/modules/client/models/store-job-info';
import { Software } from 'app/services/software.service';

@Injectable({
  providedIn: 'root',
})
export class RemoteAppService {
  url = environment.operationServer + '/api/user/my/app';
  constructor(private http: HttpClient, private storeService: StoreService, private jobService: JobService) {}
  RemoteAppList(page: number, pageSize: number) {
    const params = {
      page: page.toString(),
      count: pageSize.toString(),
    };
    return this.http.get<Result>(this.url, { params }).pipe(
      switchMap(async result => {
        console.log(result);
        return [];
      }),
    );
  }

  installApps(apps: Software[]) {
    // this.storeService.installPackages(apps);
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
  app: App;
}
