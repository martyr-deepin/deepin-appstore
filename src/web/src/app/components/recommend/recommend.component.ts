import { Component, OnInit, ViewChild } from '@angular/core';
import { Observable } from 'rxjs';

import { RecommendService, Recommend, RecommendType } from '../../services/recommend.service';

@Component({
  selector: 'app-recommend',
  templateUrl: './recommend.component.html',
  styleUrls: ['./recommend.component.scss'],
})
export class RecommendComponent implements OnInit {
  constructor(private recommendService: RecommendService) {}
  @ViewChild('myDialog') dialogRef: { nativeElement: HTMLDialogElement };
  recommend = new Recommend();
  recommendType = RecommendType;
  recommendTypeArray = Object.entries(this.recommendType);
  openDialog$: Observable<void>;
  ngOnInit() {
    this.openDialog$ = this.recommendService.onOpenRecommend().do(() => this.open());
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
    this.recommendService.recommendSubmit(this.recommend);
    console.log(this.recommend);
  }
}
