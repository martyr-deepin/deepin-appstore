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
  sectionList$: Observable<Section[]>;
  appMap: Map<string, App>;
  loadedCount = 0;

  ngOnInit() {
    this.sectionList$ = this.sectionService.getList().pipe(map(ss => ss.filter(s => s.show)));
  }
  loaded() {
    this.loadedCount++;
  }
}
