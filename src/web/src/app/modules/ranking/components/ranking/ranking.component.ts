import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs';

import { AppService } from 'app/services/app.service';
import { App } from 'app/dstore/services/app';

@Component({
  selector: 'dstore-ranking',
  templateUrl: './ranking.component.html',
  styleUrls: ['./ranking.component.scss'],
})
export class RankingComponent implements OnInit {
  constructor(private appService: AppService) {}

  apps$: Observable<App[]>;

  ngOnInit() {
    if (navigator.onLine) {
      this.apps$ = this.appService.list();
    }
  }
}
