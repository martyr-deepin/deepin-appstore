import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { map, switchMap } from 'rxjs/operators';

import { AppService } from 'app/services/app.service';

@Component({
  selector: 'dstore-tag-apps',
  templateUrl: './tag-apps.component.html',
  styleUrls: ['./tag-apps.component.scss'],
})
export class TagAppsComponent implements OnInit {
  constructor(private appService: AppService, private route: ActivatedRoute) {}
  list$ = this.route.paramMap.pipe(
    map(param => param.get('tag')),
    switchMap(
      () => this.appService.list(),
      (tag, apps) => {
        apps = apps.filter(app => app.localInfo.tags.includes(tag));
        return { tag, apps };
      },
    ),
  );
  ngOnInit() {}
}
