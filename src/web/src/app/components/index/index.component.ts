import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs';
import { map } from 'rxjs/operators';

import { SectionService } from '../../services/section.service';
import { Section, SectionType } from '../../dstore/services/section';

@Component({
  selector: 'app-index',
  templateUrl: './index.component.html',
  styleUrls: ['./index.component.scss'],
})
export class IndexComponent implements OnInit {
  constructor(private sectionService: SectionService) {}

  SectionType = SectionType;
  sectionList: Section[] = [];
  sectionList$: Observable<Section[]>;
  ngOnInit() {
    this.sectionList$ = this.sectionService.getList().pipe(map(ss => ss.filter(s => s.show)));
  }
}
