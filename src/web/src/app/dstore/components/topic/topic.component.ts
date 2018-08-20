import { Component, OnInit, Input } from '@angular/core';

import { BaseService } from '../../services/base.service';
import { Section, SectionTopic } from '../../services/section';

@Component({
  selector: 'dstore-topic',
  templateUrl: './topic.component.html',
  styleUrls: ['./topic.component.scss'],
})
export class TopicComponent implements OnInit {
  constructor() {}
  server = BaseService.serverHosts.operationServer;
  @Input() sectionIndex: number;
  @Input() section: Section;
  _topicList: SectionTopic[];
  @Input()
  set topicList(ts: SectionTopic[]) {
    this._topicList = ts.filter(t => t.show);
  }
  get topicList() {
    return this._topicList;
  }

  ngOnInit() {}
}
