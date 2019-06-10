import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'dstore-star',
  templateUrl: './star.component.html',
  styleUrls: ['./star.component.scss'],
})
export class StarComponent implements OnInit {
  @Input() rate = 0;
  @Output() rateChange = new EventEmitter<number>();
  @Input() readonly = true;

  rateHover: number;

  scoreList = new Array(10).fill(null).map((v, i) => i / 2 + 0.5);

  constructor() {}

  ngOnInit() {}
}
