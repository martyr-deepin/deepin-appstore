import { Component, OnInit } from '@angular/core';
import { StoreService } from '../../services/store.service';

@Component({
  selector: 'app-update',
  templateUrl: './update.component.html',
  styleUrls: ['./update.component.scss']
})
export class UpdateComponent implements OnInit {

  constructor(private storeService: StoreService) {}

  ngOnInit() {
    console.log('get job list');
    this.storeService.getJobList()
      .subscribe(jobsList => console.log('jobList: ', jobsList));
  }
}
