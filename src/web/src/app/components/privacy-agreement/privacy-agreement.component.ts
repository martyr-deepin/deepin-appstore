import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { PrivacyAgreementService } from 'app/services/privacy-agreement.service';

@Component({
  selector: 'dstore-privacy-agreement',
  templateUrl: './privacy-agreement.component.html',
  styleUrls: ['./privacy-agreement.component.scss'],
})
export class PrivacyAgreementComponent implements OnInit {
  @ViewChild('dialog')
  dialogRef: ElementRef<HTMLDialogElement>;
  constructor(private privateAgreement: PrivacyAgreementService) {}
  ngOnInit() {
    this.privateAgreement.onShow().subscribe(() => {
      this.dialogRef.nativeElement.showModal();
    });
  }
}
