import { Directive, ElementRef, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs';

import { HoverService } from '../services/hover.service';

@Directive({
  selector: '[dstoreHover]',
  exportAs: 'dstoreHover',
})
export class HoverDirective implements OnDestroy {
  hover = false;
  private hover$: Subscription;
  constructor(private el: ElementRef, private hoverService: HoverService) {
    this.hover$ = this.hoverService.hoverElement().subscribe(els => {
      this.hover = els.includes(this.el.nativeElement);
    });
  }
  ngOnDestroy() {
    this.hover$.unsubscribe();
  }
}
