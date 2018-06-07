import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { DomSanitizer, SafeUrl, SafeStyle } from '@angular/platform-browser';
import { trigger, state, style, transition, animate } from '@angular/animations';
import { Observable, timer, merge } from 'rxjs';
import { tap, flatMap, map } from 'rxjs/operators';

import { memoize } from 'lodash';

import { CategoryService } from '../../services/category.service';
import { Category } from '../../services/category.service';
import { BaseService } from '../../dstore/services/base.service';
import { StoreService } from '../../dstore-client.module/services/store.service';

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
    private categoryService: CategoryService,
    private sanitizer: DomSanitizer,
    private storeService: StoreService,
  ) {}
  native = BaseService.isNative;
  @ViewChild('nav') nav: ElementRef<HTMLDivElement>;
  // category list
  cs$: Observable<Category[]>;
  // download count
  dc$: Observable<number>;

  getStyle = memoize((icon: string[]) => {
    return this.sanitizer.bypassSecurityTrustStyle(
      `content: url(${icon[0]});
       --active: url(${icon[1]})`,
    );
  });
  getStyleByID = memoize((id: string) => {
    return this.sanitizer.bypassSecurityTrustStyle(
      `content: url("/assets/icons/${id}.svg");
       --active: url("/assets/icons/${id}_active.svg")`,
    );
  });
  ngOnInit() {
    this.cs$ = this.categoryService.list();
    this.dc$ = merge(this.storeService.getJobList(), this.storeService.jobListChange()).pipe(
      map(jobs => jobs.filter(job => job.includes('install')).length),
    );
  }
  mousewheel(event: WheelEvent) {
    const nav = this.nav.nativeElement;
    if (event.wheelDeltaY > 0) {
      if (nav.scrollTop === 0) {
        event.preventDefault();
      }
    } else {
      if (nav.scrollTop + nav.clientHeight === nav.scrollHeight) {
        event.preventDefault();
      }
    }
  }
}
