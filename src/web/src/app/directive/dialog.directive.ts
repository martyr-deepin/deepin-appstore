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
    console.log(e, el);
    if (
      (e.wheelDeltaY < 0 &&
        el.scrollTop + el.offsetHeight >= this.elementRef.nativeElement.scrollHeight) ||
      (e.wheelDeltaY > 0 && el.scrollTop === 0)
    ) {
      e.preventDefault();
    }
  }
  @HostListener('click', ['$event'])
  click(e: Event) {
    if (e.srcElement.nodeName === 'DIALOG') {
      this.elementRef.nativeElement.close();
    }
  }
}
