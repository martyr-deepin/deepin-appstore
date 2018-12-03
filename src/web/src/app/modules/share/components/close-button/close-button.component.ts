import { Component, OnInit, Input, ElementRef } from '@angular/core';

@Component({
  selector: 'dstore-close-button',
  templateUrl: './close-button.component.html',
  styleUrls: ['./close-button.component.scss'],
})
export class CloseButtonComponent {
  @Input()
  dialog: HTMLDialogElement;
  click() {
    if (this.dialog && this.dialog.open) {
      this.dialog.close();
    }
  }
}
