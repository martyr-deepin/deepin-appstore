import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs';
import { map } from 'rxjs/operators';

import { SectionService } from '../../services/section.service';
import { Section, SectionType } from '../../dstore/services/section';
import { App } from '../../services/app.service';
import { AppService } from '../../services/app.service';

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
  appFilter = this._appFilter.bind(this);
  ngOnInit() {
    this.appService.appMap().subscribe(appMap => {
      this.appMap = appMap;
      this.sectionList$ = this.sectionService.getList().pipe(map(ss => ss.filter(s => s.show)));
    });
  }
  _appFilter(appName: string): boolean {
    return this.appMap.has(appName);
  }
}
