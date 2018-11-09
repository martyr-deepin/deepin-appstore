import { Component, OnInit, ViewChild, ElementRef, Input } from '@angular/core';
import { Router, NavigationStart } from '@angular/router';
import PerfectScrollbar from 'perfect-scrollbar';
import { filter, pairwise } from 'rxjs/operators';

@Component({
  selector: 'dstore-scrollbar',
  templateUrl: './scrollbar.component.html',
  styleUrls: ['./scrollbar.component.scss'],
})
export class ScrollbarComponent implements OnInit {
  constructor(private router: Router) {}
  @ViewChild('scrollbar')
  scrollbarEl: ElementRef<HTMLDivElement>;
  @Input()
  savePosition: boolean;
  position = new Map<string, [number, number]>();

  getPos(): [number, number] {
    return [this.el.scrollTop, this.el.scrollLeft];
  }
  setPos(pos: [number, number] = [0, 0]) {
    [this.el.scrollTop, this.el.scrollLeft] = pos;
  }
  get el() {
    return this.scrollbarEl.nativeElement;
  }
  ngOnInit() {
    const scrollbar = new PerfectScrollbar(this.el, {
      suppressScrollY: false,
      suppressScrollX: true,
      wheelPropagation: false,
    });
    this.router.events
      .pipe(
        filter(event => event instanceof NavigationStart),
        pairwise(),
      )
      .subscribe(([oldRoute, newRoute]: [NavigationStart, NavigationStart]) => {
        if (this.savePosition) {
          this.position.set(oldRoute.url, this.getPos());
        }
        window['requestIdleCallback'](() => {
          this.setPos(this.position.get(newRoute.url));
          scrollbar.update();
        });
      });
  }
}
