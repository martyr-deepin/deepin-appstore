import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs';
import { switchMap } from 'rxjs/operators';

import { App, AppService } from 'app/services/app.service';
import { ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-more',
  templateUrl: './more.component.html',
  styleUrls: ['./more.component.scss'],
})
export class MoreComponent implements OnInit {
  constructor(private route: ActivatedRoute, private appService: AppService) {}
  title: string[];
  apps$: Observable<App[]>;
  ngOnInit() {
    this.apps$ = this.route.paramMap.pipe(
      switchMap(paramMap => {
        this.title = paramMap.get('title').split(',');
        return this.appService.getApps(paramMap.get('apps').split(','));
      }),
    );
  }
}
