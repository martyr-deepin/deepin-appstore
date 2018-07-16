import { Directive, HostListener, ElementRef } from '@angular/core';
import { JsonpClientBackend } from '@angular/common/http';

@Directive({
  selector: 'dialog',
})
export class DialogDirective {
  constructor(private elementRef: ElementRef<HTMLDialogElement>) {}
  @HostListener('mousewheel', ['$event'])
  mousewheel(e: Event) {
    e.preventDefault();
  }
  @HostListener('click', ['$event'])
  click(e: Event) {
    if (e.srcElement.nodeName === 'DIALOG') {
      this.elementRef.nativeElement.close();
    }
  }
}
