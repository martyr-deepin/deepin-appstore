import { Directive, Output, ElementRef, EventEmitter, OnInit, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs';
import { BrowserService } from '../services/browser.service';

@Directive({
  selector: '[dstoreResize]',
})
export class ResizeDirective implements OnInit, OnDestroy {
  @Output()
  resize = new EventEmitter<{ width: number; height: number }>(true);
  sub: Subscription;
  constructor(private elRef: ElementRef<HTMLElement>, private browserService: BrowserService) {}
  getSize() {
    const el = this.elRef.nativeElement;
    return {
      width: el.clientWidth,
      height: el.clientHeight,
    };
  }
  ngOnInit() {
    this.sub = this.browserService.windowSize$.subscribe(() => {
      this.resize.emit(this.getSize());
    });
    this.resize.emit(this.getSize());
  }
  ngOnDestroy() {
    if (this.sub) {
      this.sub.unsubscribe();
    }
  }
}
