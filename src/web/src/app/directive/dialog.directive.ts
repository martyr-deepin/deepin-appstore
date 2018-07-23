import { Directive, HostListener, ElementRef } from '@angular/core';
import { JsonpClientBackend } from '@angular/common/http';

@Directive({
  selector: 'dialog',
})
export class DialogDirective {
  constructor(private elementRef: ElementRef<HTMLDialogElement>) {}
  @HostListener('mousewheel', ['$event'])
  mousewheel(e: MouseWheelEvent) {
    e.stopPropagation();

    const el = this.elementRef.nativeElement;
    if (
      (e.wheelDeltaY < 0 &&
        el.scrollHeight - el.scrollTop <= el.clientHeight + loginDialogMargin) ||
      (e.wheelDeltaY > 0 && el.scrollTop === 0)
    ) {
      e.preventDefault();
    }
  }
  @HostListener('click', ['$event'])
  click(e: Event) {
    if (e.srcElement.nodeName === 'DIALOG' && this.elementRef.nativeElement.open) {
      this.elementRef.nativeElement.close();
    }
  }
}
const loginDialogMargin = 4;
