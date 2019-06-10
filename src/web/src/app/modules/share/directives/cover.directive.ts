import { Directive, HostListener, ElementRef, Input } from '@angular/core';

@Directive({
  selector: '[dstoreCover]',
})
export class CoverDirective {
  @Input('dstoreCover') type: 'cover' | 'icon';
  constructor(private el: ElementRef<HTMLImageElement>) {}
  @HostListener('error') onError() {
    switch (this.type) {
      case 'cover':
        this.el.nativeElement.src = '/assets/images/default_cover.png';
        break;
      case 'icon':
        this.el.nativeElement.src = '/assets/images/default_icon.svg';
    }
  }
}
