import { Directive, OnInit, HostListener, HostBinding, ElementRef, Input } from '@angular/core';

@Directive({
  selector: '[dstoreCover]',
})
export class CoverDirective implements OnInit {
  @Input('dstoreCover') type: 'cover' | 'icon';
  constructor(private el: ElementRef<HTMLImageElement>) {}
  ngOnInit() {
    if (!this.el.nativeElement.src) {
      this.onError();
    }
  }
  @HostBinding('style.border-radius.px') borderRadius = 4;
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
