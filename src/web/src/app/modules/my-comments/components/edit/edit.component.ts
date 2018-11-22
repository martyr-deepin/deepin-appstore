import {
  Component,
  OnInit,
  Input,
  Output,
  EventEmitter,
  ViewChild,
  ElementRef,
} from '@angular/core';
import { UserComment } from '../../services/comments.service';

@Component({
  selector: 'dstore-edit',
  templateUrl: './edit.component.html',
  styleUrls: ['./edit.component.scss'],
})
export class EditComponent implements OnInit {
  @ViewChild('dialog')
  dialogRef: ElementRef<HTMLDialogElement>;
  deleteConfirm: boolean;
  appName: string;
  content: string;
  rate: number;
  version: string;
  _comment: UserComment;
  @Output()
  close = new EventEmitter<boolean>();
  @Input()
  set comment(c: UserComment) {
    if (!this.dialogRef.nativeElement.open) {
      this.dialogRef.nativeElement.showModal();
    }
    this._comment = c;
    this.appName = c.appName;
    this.content = c.content;
    this.rate = c.rate / 2;
    this.version = c.version;
  }
  constructor() {}

  ngOnInit() {
    this.dialogRef.nativeElement.addEventListener('close', () => {
      this.closed();
    });
  }
  closed(changed: boolean = false) {
    this.close.emit(changed);
  }
}
