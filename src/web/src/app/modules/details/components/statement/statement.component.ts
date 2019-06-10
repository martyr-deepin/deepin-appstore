import { Component, OnInit } from '@angular/core';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';
import { AgreementService } from 'app/services/agreement.service';

@Component({
  selector: 'app-statement',
  templateUrl: './statement.component.html',
  styleUrls: ['./statement.component.scss'],
})
export class StatementComponent implements OnInit {
  constructor(private agreement: AgreementService) {}

  statement$ = this.agreement.donation();

  ngOnInit() {}
  click(e: Event) {
    e.stopPropagation();
    e.preventDefault();
    if (e.target['href']) {
      DstoreObject.openURL(e.target['href']);
    }
  }
}
