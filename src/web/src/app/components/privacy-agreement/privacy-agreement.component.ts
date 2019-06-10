import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { PrivacyAgreementService } from 'app/services/privacy-agreement.service';
import { AgreementService } from 'app/services/agreement.service';
import { DstoreObject } from 'app/modules/client/utils/dstore-objects';

@Component({
  selector: 'dstore-privacy-agreement',
  templateUrl: './privacy-agreement.component.html',
  styleUrls: ['./privacy-agreement.component.scss'],
})
export class PrivacyAgreementComponent implements OnInit {
  @ViewChild('dialog')
  dialogRef: ElementRef<HTMLDialogElement>;
  constructor(private privateAgreement: PrivacyAgreementService, private agreenment: AgreementService) {}
  private$ = this.agreenment.privacy();
  ngOnInit() {
    this.privateAgreement.onShow().subscribe(() => {
      this.dialogRef.nativeElement.showModal();
    });
  }
  click(e: Event) {
    if (e.target['href']) {
      DstoreObject.openURL(e.target['href']);
    }
  }
}
