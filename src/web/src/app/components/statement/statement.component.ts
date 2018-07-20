import { Component, OnInit } from '@angular/core';
import { DstoreObject } from '../../dstore-client.module/utils/dstore-objects';

@Component({
  selector: 'app-statement',
  templateUrl: './statement.component.html',
  styleUrls: ['./statement.component.scss'],
})
export class StatementComponent implements OnInit {
  constructor() {}
  ngOnInit() {}
  open(url: string) {
    DstoreObject.openURL(url);
  }
}
