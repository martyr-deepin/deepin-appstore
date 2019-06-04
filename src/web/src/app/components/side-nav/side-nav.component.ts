import {
  Component,
  OnInit,
  ViewChild,
  ElementRef,
  ViewChildren,
  QueryList,
} from '@angular/core';
import { DomSanitizer } from '@angular/platform-browser';
import { trigger, style, transition, animate } from '@angular/animations';
import { RouterLinkActive, RouterLink } from '@angular/router';
import { Observable } from 'rxjs';
import { map } from 'rxjs/operators';

import { CategoryService } from '../../services/category.service';
import { Category } from '../../services/category.service';
import { BaseService } from '../../dstore/services/base.service';
import { StoreJobType } from 'app/modules/client/models/store-job-info';
import { JobService } from 'app/services/job.service';

@Component({
  selector: 'app-side-nav',
  templateUrl: './side-nav.component.html',
  styleUrls: ['./side-nav.component.scss'],
  animations: [
    trigger('dlcIn', [
      transition(':enter', [style({ width: 0 }), animate(100)]),
      transition(':leave', [animate(100, style({ width: 0 }))]),
    ]),
  ],
})
export class SideNavComponent implements OnInit {
  constructor(
    private sanitizer: DomSanitizer,
    private categoryService: CategoryService,
    private jobService: JobService,
  ) {}
  native = BaseService.isNative;
  // category list
  cs$: Observable<Category[]>;
  // download count
  dc$: Observable<number>;

  ngOnInit() {
    this.cs$ = this.categoryService.list();
    const CountType = [StoreJobType.install, StoreJobType.download];
    this.dc$ = this.jobService.jobsInfo().pipe(
      map(infoList => {
        return infoList.filter(info => CountType.includes(info.type)).length;
      }),
    );
  }

  getStyle(icons: string[]) {
    return this.sanitizer.bypassSecurityTrustStyle(
      icons.map(url => `url(${url})`).join(','),
    );
  }
}
