import { Component, OnInit } from '@angular/core';
import { SectionItemBase } from '../section-item-base';
import { SectionTopic } from 'app/dstore/services/section';

@Component({
  selector: 'index-topic',
  templateUrl: './topic.component.html',
  styleUrls: ['./topic.component.scss'],
})
export class TopicComponent extends SectionItemBase implements OnInit {
  constructor() {
    super();
  }
  topics: SectionTopic[];
  ngOnInit() {
    this.topics = (this.section.items as SectionTopic[])
      .filter(topic => topic.show)
      .map(topic => {
        return topic;
      });
  }
}
