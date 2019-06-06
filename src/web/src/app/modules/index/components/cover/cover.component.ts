import { Component, OnInit } from '@angular/core';
import { Section, SectionApp } from 'app/dstore/services/section';
import { SoftwareService, Software } from 'app/services/software.service';
import { SectionItemBase } from '../section-item-base';

@Component({
  selector: 'index-cover',
  templateUrl: './cover.component.html',
  styleUrls: ['./cover.component.scss'],
})
export class CoverComponent extends SectionItemBase implements OnInit {
  constructor(private softwareService: SoftwareService) {
    super();
  }
  ngOnInit() {
    const apps: SectionApp[] = this.section.items;
    this.softs$ = this.softwareService
      .list({ names: apps.filter(app => app.show).map(app => app.name) })
      .finally(() => this.loaded.emit(true));
  }
}
