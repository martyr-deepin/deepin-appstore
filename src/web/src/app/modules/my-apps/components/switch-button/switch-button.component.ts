import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'dstore-switch-button',
  templateUrl: './switch-button.component.html',
  styleUrls: ['./switch-button.component.scss'],
})
export class SwitchButtonComponent implements OnInit {
  @Output()
  valueChange = new EventEmitter<boolean>();
  @Input()
  value = false;

  constructor() {}
  ngOnInit() {
    console.log(this.value);
  }
  change() {
    this.value = !this.value;
    this.valueChange.emit(this.value);
  }
}
