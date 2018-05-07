import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';

import { get } from 'lodash';
import { Observable } from 'rxjs';

import { SectionTopic } from '../../dstore/services/section';
import { SectionService } from '../../services/section.service';
import { BaseService } from '../../dstore/services/base.service';
import { AppService } from '../../services/app.service';

import { App } from '../../dstore/services/app';
import { SortOrder } from '../app-title/app-title.component';

@Component({
  selector: 'app-topic',
  templateUrl: './topic.component.html',
  styleUrls: ['./topic.component.scss'],
})
export class TopicComponent implements OnInit {
  server = BaseService.serverHosts.operationServer;
  constructor(
    private route: ActivatedRoute,
    private appService: AppService,
    private sectionService: SectionService,
  ) {}

  sortBy = SortOrder.Downloads;
  topic$: Observable<SectionTopic>;
  apps$: Observable<App[]>;
  ngOnInit() {
    this.topic$ = this.route.paramMap
      .mergeMap(param => {
        const sectionIndex = parseInt(param.get('section'), 10);
        const topicIndex = parseInt(param.get('topic'), 10);
        return this.sectionService
          .getList()
          .map(sectionList => sectionList[sectionIndex].items[topicIndex]);
      })
      .do(t => console.log('topic', t));

    this.apps$ = this.topic$.mergeMap(topic => {
      const appNameList = topic.apps.map(app => app.name);
      return this.appService.list().map(apps => apps.filter(app => appNameList.includes(app.name)));
    });
  }
}
