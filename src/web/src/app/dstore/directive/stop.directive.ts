import { Directive, HostListener } from '@angular/core';

@Directive({
  selector: '[click-stop]',
})
export class StopDirective {
  constructor() {}
  @HostListener('click', ['$event'])
  public onClick(event: any): void {
    event.stopPropagation();
  }
}
