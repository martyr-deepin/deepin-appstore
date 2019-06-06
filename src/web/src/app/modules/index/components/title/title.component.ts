import { Component, OnInit, Input } from '@angular/core';

@Component({
  selector: 'index-title',
  templateUrl: './title.component.html',
  styleUrls: ['./title.component.scss'],
})
export class TitleComponent implements OnInit {
  constructor() {}
  @Input() titles: string[];
  @Input() more: boolean;
  ngOnInit() {}
}
