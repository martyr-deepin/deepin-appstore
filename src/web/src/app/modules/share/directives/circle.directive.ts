import {
  Directive,
  Input,
  ElementRef,
  OnChanges,
  SimpleChanges,
} from '@angular/core';
/*********** Deprecated ***********/
/*********** Qcef version do not support 'conic-gradient'***********/
@Directive({
  selector: '[dstoreCircle]',
})
export class CircleDirective implements OnChanges {
  @Input('dstoreCircle') value = 0;
  constructor(private elRef: ElementRef<HTMLDivElement>) {}

  ngOnChanges() {
    const deg = 360 * this.value;
    const maskImage = `conic-gradient(red ${deg}deg, transparent ${deg}deg)`;
    console.log(maskImage);
    this.elRef.nativeElement.style.maskImage = maskImage;
  }
}
