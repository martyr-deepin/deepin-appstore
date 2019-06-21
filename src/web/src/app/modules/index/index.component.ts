import { Component, OnInit } from '@angular/core';

import { SectionService, SectionType } from './services/section.service';

@Component({
  selector: 'app-index',
  templateUrl: './index.component.html',
  styleUrls: ['./index.component.scss'],
})
export class IndexComponent implements OnInit {
  constructor(private sectionService: SectionService) {}
  SectionType = SectionType;
  sectionList$ = this.sectionService.getList();
  loadedCount = 0;

  ngOnInit() {}
  loaded() {
    this.loadedCount++;
  }
}
