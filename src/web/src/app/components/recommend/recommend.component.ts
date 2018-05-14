import { Component, OnInit, ViewChild } from '@angular/core';
import { Observable } from 'rxjs';
import { map } from 'rxjs/operators';

import { RecommendService, Recommend, RecommendType } from '../../services/recommend.service';
import { MaterializeService } from '../../dstore/services/materialize.service';

@Component({
  selector: 'app-recommend',
  templateUrl: './recommend.component.html',
  styleUrls: ['./recommend.component.scss'],
})
export class RecommendComponent implements OnInit {
  constructor(
    private recommendService: RecommendService,
    private materializeService: MaterializeService,
  ) {}
  @ViewChild('myDialog') dialogRef: { nativeElement: HTMLDialogElement };
  recommend = new Recommend();
  recommendType = RecommendType;
  openDialog$: Observable<void>;
  ngOnInit() {
    this.openDialog$ = this.recommendService.onOpenRecommend().pipe(map(() => this.open()));
  }
  open() {
    if (!this.dialogRef.nativeElement.open) {
      this.dialogRef.nativeElement.showModal();
    }
  }
  close() {
    this.dialogRef.nativeElement.close();
  }
  submit() {
    this.recommendService.recommendSubmit(this.recommend).subscribe(
      () => {
        this.materializeService.toastSuccess('投递成功');
        this.dialogRef.nativeElement.close();
        this.recommend = new Recommend();
      },
      () => {
        this.materializeService.toastSuccess('投递失败');
      },
    );
  }
}
