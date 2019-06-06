import { Component, OnInit } from '@angular/core';

import { Observable, combineLatest } from 'rxjs';
import { map } from 'rxjs/operators';

import { SectionService } from './services/section.service';
import { Section, SectionType } from 'app/dstore/services/section';
import { App } from 'app/services/app.service';
import { AppService } from 'app/services/app.service';

@Component({
  selector: 'app-index',
  templateUrl: './index.component.html',
  styleUrls: ['./index.component.scss'],
})
export class IndexComponent implements OnInit {
  constructor(private sectionService: SectionService, private appService: AppService) {}
  SectionType = SectionType;
  sectionList$ = this.sectionService.getList();
  appMap: Map<string, App>;
  loadedCount = 1;

  ngOnInit() {}
  loaded() {
    this.loadedCount++;
  }
}
