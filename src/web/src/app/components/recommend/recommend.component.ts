import { Component, OnInit, ViewChild } from '@angular/core';
import { Observable } from 'rxjs';
import { map } from 'rxjs/operators';

import { RecommendService, Recommend } from '../../services/recommend.service';
import { NotifyService } from '../../services/notify.service';
import { NotifyType } from '../../services/notify.model';
import { NgForm } from '@angular/forms';

@Component({
  selector: 'app-recommend',
  templateUrl: './recommend.component.html',
  styleUrls: ['./recommend.component.scss'],
})
export class RecommendComponent implements OnInit {
  constructor(private recommendService: RecommendService, private notifyService: NotifyService) {}
  @ViewChild('myDialog') dialogRef: { nativeElement: HTMLDialogElement };
  @ViewChild('myForm') form: NgForm;
  recommend = new Recommend();
  openDialog$: Observable<void>;

  ngOnInit() {
    this.openDialog$ = this.recommendService.onOpenRecommend().pipe(map(() => this.open()));
  }

  open() {
    if (!this.dialogRef.nativeElement.open) {
      this.recommend = new Recommend();
      this.dialogRef.nativeElement.showModal();
    }
  }

  close() {
    this.dialogRef.nativeElement.close();
  }

  submit() {
    if (this.form.invalid) {
      Object.values(this.form.controls).forEach(control => control.markAsDirty());
      return;
    }
    this.recommendService.recommendSubmit(this.recommend).subscribe(
      () => {
        this.dialogRef.nativeElement.close();
        this.recommend = new Recommend();
        this.notifyService.success(NotifyType.Recommend);
      },
      () => {
        this.notifyService.error(NotifyType.Recommend);
      },
    );
  }
}
