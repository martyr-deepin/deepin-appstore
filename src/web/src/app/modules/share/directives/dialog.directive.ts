import { Directive, HostListener, ElementRef, Input } from '@angular/core';

@Directive({
  selector: 'dialog',
})
export class DialogDirective {
  constructor(private elementRef: ElementRef<HTMLDialogElement>) {}

  @HostListener('mousedown', ['$event'])
  click(e: Event) {
    if (e.srcElement.nodeName === 'DIALOG' && this.elementRef.nativeElement.open) {
      this.elementRef.nativeElement.close();
    }
  }
}
