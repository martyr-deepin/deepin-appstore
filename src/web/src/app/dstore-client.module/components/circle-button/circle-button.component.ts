import { Component, OnInit, Input, Output, EventEmitter, HostListener } from '@angular/core';

import { Subject } from 'rxjs';
import { tap, throttleTime } from 'rxjs/operators';

@Component({
  selector: 'dstore-circle-button',
  templateUrl: './circle-button.component.html',
  styleUrls: ['./circle-button.component.scss'],
})
export class CircleButtonComponent implements OnInit {
  constructor() {}
  @Input() running = true;
  @Input() progress = 0;
  @Input() disabled: boolean;

  @Output() runningChange = new EventEmitter<boolean>();
  click$ = new Subject();

  @HostListener('click', ['$event'])
  click(e: Event) {
    e.stopPropagation();
    if (!this.disabled) {
      this.click$.next();
    }
  }

  ngOnInit() {
    this.click$
      .pipe(
        tap(() => {
          this.running = !this.running;
        }),
        throttleTime(500),
      )
      .subscribe(() => {
        this.runningChange.emit(this.running);
      });
  }
}
