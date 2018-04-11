import { Component, OnInit } from '@angular/core';

import { SectionService } from '../../services/section.service';
import { Section, SectionType } from '../../dstore/services/section';

@Component({
  selector: 'app-index',
  templateUrl: './index.component.html',
  styleUrls: ['./index.component.scss']
})
export class IndexComponent implements OnInit {
  constructor(private sectionService: SectionService) {}

  SectionType = SectionType;
  sectionList: Section[] = [];
  ngOnInit() {
    this.sectionService.getList().subscribe(ss => (this.sectionList = ss));
  }
}
