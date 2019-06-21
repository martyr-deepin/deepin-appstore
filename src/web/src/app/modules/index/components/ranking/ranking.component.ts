import { Component, OnInit } from '@angular/core';
import { SoftwareService } from 'app/services/software.service';
import { SectionItemBase } from '../section-item-base';

@Component({
  selector: 'index-ranking',
  templateUrl: './ranking.component.html',
  styleUrls: ['./ranking.component.scss'],
})
export class RankingComponent extends SectionItemBase implements OnInit {
  constructor(private softwareService: SoftwareService) {
    super();
  }
  ngOnInit() {
    this.load().finally(() => {
      this.loaded.emit(true);
    });
  }
  async load() {
    const ranking = this.section.ranking || { category: '', count: 20 };
    let softs = [];
    for (let offset = 0; softs.length < ranking.count; offset += 10) {
      const list = await this.softwareService.list({ category: ranking.category, offset, limit: 10 });
      softs = [...softs, ...list].slice(0, ranking.count);
    }
    this.softs$ = Promise.resolve(softs);
  }
}
