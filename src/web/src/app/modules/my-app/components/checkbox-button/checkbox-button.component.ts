import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'dstore-checkbox-button',
  templateUrl: './checkbox-button.component.html',
  styleUrls: ['./checkbox-button.component.scss'],
})
export class CheckboxButtonComponent implements OnInit {
  @Output()
  valueChange = new EventEmitter<boolean>();
  @Input()
  value = false;
  @Input()
  disabled = false;

  constructor() {}
  ngOnInit() {}
  change() {
    if (this.disabled) {
      return;
    }
    this.value = !this.value;
    this.valueChange.emit(this.value);
  }
}
