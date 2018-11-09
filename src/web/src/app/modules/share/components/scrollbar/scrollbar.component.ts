import { Component, OnInit, OnDestroy, ViewChild, ElementRef, Input } from '@angular/core';
import { Router, RouterEvent, NavigationStart, NavigationEnd } from '@angular/router';
import PerfectScrollbar from 'perfect-scrollbar';
import { Subscription } from 'rxjs';
import { filter, pairwise } from 'rxjs/operators';

@Component({
  selector: 'dstore-scrollbar',
  templateUrl: './scrollbar.component.html',
  styleUrls: ['./scrollbar.component.scss'],
})
export class ScrollbarComponent implements OnInit, OnDestroy {
  constructor(private router: Router) {}
  @ViewChild('scrollbar')
  scrollbarEl: ElementRef<HTMLDivElement>;
  @Input()
  savePosition: boolean;
  position = new Map<number, [number, number]>();
  restored: Subscription;

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

    let restoreID: number;
    this.restored = this.router.events.subscribe((event: RouterEvent) => {
      if (event instanceof NavigationStart) {
        if (this.savePosition) {
          this.position.set(this.router['lastSuccessfulId'], this.getPos());

          if (event.restoredState) {
            restoreID = event.restoredState.navigationId;
          } else {
            restoreID = null;
          }
        }
      } else if (event instanceof NavigationEnd) {
        window['requestIdleCallback'](() => {
          scrollbar.update();
          if (restoreID) {
            const pos = this.position.get(restoreID);
            setTimeout(() => {
              console.log('scrollbar restore', pos);
              this.setPos(pos);
            }, 200);
          } else {
            this.setPos();
          }
        });
      }
    });
  }
  ngOnDestroy() {
    if (this.restored) {
      this.restored.unsubscribe();
    }
  }
}
