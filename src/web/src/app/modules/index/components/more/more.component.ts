import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { first, switchMap } from 'rxjs/operators';
import { KeyvalueService } from 'app/services/keyvalue.service';
import { Section, SectionApp } from '../../services/section.service';
import { SoftwareService } from 'app/services/software.service';
@Component({
  selector: 'index-more',
  templateUrl: './more.component.html',
  styleUrls: ['./more.component.scss'],
})
export class MoreComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private keyvalue: KeyvalueService,
    private softwareService: SoftwareService,
  ) {}
  section$ = this.route.paramMap
    .pipe(first())
    .toPromise()
    .then(param => param.get('key'))
    .then(key => this.keyvalue.get<Section>(key));
  title$ = this.section$.then(section => section.title);
  softs$ = this.route.queryParamMap.pipe(
    switchMap(async query => {
      const section = await this.section$;
      const names = (<SectionApp[]>section.items).filter(app => app.show).map(app => app.name);
      let list = await this.softwareService.list({ names });
      list = list.filter(Boolean);
      switch (query.get('order')) {
        case 'download':
          return list.sort((a, b) => a.stat.download - b.stat.download);
        case 'score':
          return list.sort((a, b) => a.stat.score * a.stat.score_count - b.stat.score * b.stat.score_count);
        default:
          return list;
      }
    }),
  );
  ngOnInit() {}
}
