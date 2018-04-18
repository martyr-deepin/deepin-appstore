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
    // this.storeService.getJobList()
    //   .subscribe(jobsList => console.log('jobList: ', jobsList));
    this.storeService.installPackage('mariadb-server')
      .subscribe(job => {
        console.log('job path: ', job);
        window.setTimeout(() => {
          this.storeService.pauseJob(job);
          this.storeService.getJobInfo(job)
            .subscribe(jobInfo => {
              console.log('job info:', jobInfo);
            });
        }, 200);
      });
  }
}
