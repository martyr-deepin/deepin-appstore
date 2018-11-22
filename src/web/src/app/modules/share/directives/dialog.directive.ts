import { Directive, HostListener, ElementRef, Input } from '@angular/core';

@Directive({
  selector: 'dialog',
})
export class DialogDirective {
  constructor(private elementRef: ElementRef<HTMLDialogElement>) {}

  @HostListener('mousedown', ['$event'])
  click(e: MouseEvent) {
    if (e.srcElement === this.elementRef.nativeElement && this.elementRef.nativeElement.open) {
      const rect = this.elementRef.nativeElement.getBoundingClientRect();
      if (e.x < rect.left || e.x > rect.right || e.y < rect.top || e.y > rect.bottom) {
        this.elementRef.nativeElement.close();
      }
    }
  }
  @HostListener('mousewheel', ['$event'])
  wheel(e: Event) {
    e.stopPropagation();
  }
}
