import { Directive, HostListener, ElementRef, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs';

import { HoverService } from '../services/hover.service';

@Directive({
  selector: '[dstoreHover]',
  exportAs: 'dstoreHover',
})
export class HoverDirective {
  hover = false;
  constructor() {}
  @HostListener('mouseover') mouseover() {
    this.hover = true;
  }
  @HostListener('mouseout') mouseout() {
    this.hover = false;
    console.log('out');
  }
}
