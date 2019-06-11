import { Component, OnInit } from '@angular/core';
import { SectionItemBase } from '../section-item-base';
import { SectionTopic } from '../../services/section.service';
import { KeyvalueService } from 'app/services/keyvalue.service';

@Component({
  selector: 'index-topic',
  templateUrl: './topic.component.html',
  styleUrls: ['./topic.component.scss'],
})
export class TopicComponent extends SectionItemBase implements OnInit {
  constructor(private keyvalue: KeyvalueService) {
    super();
  }
  ids = new Map<number, string>();
  topics: SectionTopic[];
  ngOnInit() {
    this.topics = (this.section.items as SectionTopic[])
      .filter(topic => topic.show)
      .map((topic, index) => {
        const id = this.keyvalue.add(topic);
        this.ids.set(index, id);
        return topic;
      });
  }
}
