import { Directive, HostListener, ElementRef, Input } from '@angular/core';

@Directive({
  selector: 'dialog',
})
export class DialogDirective {
  constructor(private elementRef: ElementRef<HTMLDialogElement>) {}

  @HostListener('mousedown', ['$event'])
  click(e: Event) {
    if (e.srcElement === this.elementRef.nativeElement && this.elementRef.nativeElement.open) {
      this.elementRef.nativeElement.close();
    }
  }
  @HostListener('mousewheel', ['$event'])
  wheel(e: Event) {
    e.stopPropagation();
  }
}
