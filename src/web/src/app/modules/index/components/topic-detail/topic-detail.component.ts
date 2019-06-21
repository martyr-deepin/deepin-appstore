import { Component, OnInit, HostBinding } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { first, switchMap } from 'rxjs/operators';
import { KeyvalueService } from 'app/services/keyvalue.service';
import { Section, SectionApp, SectionTopic } from '../../services/section.service';
import { SoftwareService } from 'app/services/software.service';

@Component({
  selector: 'dstore-topic-detail',
  templateUrl: './topic-detail.component.html',
  styleUrls: ['./topic-detail.component.scss'],
})
export class TopicDetailComponent implements OnInit {
  constructor(
    private route: ActivatedRoute,
    private keyvalue: KeyvalueService,
    private softwareService: SoftwareService,
  ) {}
  @HostBinding('style.background-color') bgColor: string;
  section$ = this.route.paramMap
    .pipe(first())
    .toPromise()
    .then(param => param.get('key'))
    .then(key => this.keyvalue.get<SectionTopic>(key));
  softs$ = this.section$.then(section => {
    this.bgColor = section.backgroundColor;
    console.log(section);
    return this.softwareService.list({ names: section.apps.filter(app => app.show).map(app => app.name) });
  });
  ngOnInit() {}
}
