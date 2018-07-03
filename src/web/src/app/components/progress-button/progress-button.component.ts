import { Component, OnInit, Input } from '@angular/core';

@Component({
  selector: 'app-progress-button',
  templateUrl: './progress-button.component.html',
  styleUrls: ['./progress-button.component.scss'],
})
export class ProgressButtonComponent implements OnInit {
  constructor() {}
  @Input() text = '';
  @Input() progress = 0.5;
  @Input() disabled = false;
  ngOnInit() {}
}
