import { Directive, HostListener } from '@angular/core';

@Directive({
  selector: '[appHover]',
  exportAs: 'appHover',
})
export class HoverDirective {
  constructor() {}
  _hover = false;
  get Hover() {
    return this._hover;
  }
  @HostListener('mouseenter')
  private onMouseEnter() {
    this._hover = true;
  }
  @HostListener('mouseleave')
  private onMouseLeave() {
    this._hover = false;
  }
}
