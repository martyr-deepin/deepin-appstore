import { Directive, HostListener, ElementRef, Input } from '@angular/core';
import { JsonpClientBackend } from '@angular/common/http';

@Directive({
  selector: 'dialog',
})
export class DialogDirective {
  constructor(private elementRef: ElementRef<HTMLDialogElement>) {}

  @HostListener('mousedown', ['$event'])
  click(e: Event) {
    console.log(e);
    if (e.srcElement.nodeName === 'DIALOG' && this.elementRef.nativeElement.open) {
      this.elementRef.nativeElement.close();
    }
  }
}
const loginDialogMargin = 4;
