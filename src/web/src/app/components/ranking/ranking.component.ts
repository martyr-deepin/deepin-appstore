import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs';

import { AppService } from '../../services/app.service';
import { App } from '../../dstore/services/app';

@Component({
  selector: 'app-ranking',
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
