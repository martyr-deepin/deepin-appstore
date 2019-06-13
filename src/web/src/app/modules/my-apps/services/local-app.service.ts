import { Injectable } from '@angular/core';
import { switchMap, map } from 'rxjs/operators';
import { chunk } from 'lodash';

import { JobService } from 'app/services/job.service';
import { StoreService } from 'app/modules/client/services/store.service';
import { StoreJobType } from 'app/modules/client/models/store-job-info';
import { environment } from 'environments/environment';
import { SoftwareService, Software } from 'app/services/software.service';

@Injectable({
  providedIn: 'root',
})
export class LocalAppService {
  metadataServer = environment.metadataServer;
  constructor(
    private jobService: JobService,
    private storeService: StoreService,
    private softwareService: SoftwareService,
  ) {}

  list({ pageIndex = 0, pageSize = 20 }) {
    return this.jobService.jobList().pipe(
      switchMap(() => this.storeService.InstalledPackages()),
      switchMap(async installed => {
        const pkgs = await this.softwareService.packages;
        const names = ([] as string[]).concat(
          ...installed
            .sort((a, b) => b.installedTime - a.installedTime)
            .map(pkg => pkgs[pkg.packageURI])
            .filter(Boolean),
        );
        const list = await this.softwareService.list({
          names: chunk(names, pageSize)[pageIndex],
          filterPackage: false,
          filterStat: false,
        });
        return { total: names.length, page: pageIndex, list };
      }),
    );
  }

  removingList() {
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

  removeLocal(soft: Software) {
    this.softwareService.remove(soft);
  }
}
