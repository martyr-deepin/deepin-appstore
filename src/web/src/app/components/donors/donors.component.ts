import { Component, OnInit, Input, OnChanges } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { map, switchMap } from 'rxjs/operators';
import { DonorsService } from '../../services/donors.service';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-donors',
  templateUrl: './donors.component.html',
  styleUrls: ['./donors.component.scss'],
})
export class DonorsComponent implements OnInit, OnChanges {
  constructor(
    private route: ActivatedRoute,
    private donorsService: DonorsService,
    private auth: AuthService,
  ) {}
  @Input() appName: string;
  donors = [];
  total = 0;
  ngOnInit() {}
  ngOnChanges() {
    this.donorsService
      .getDonation(this.appName)
      .pipe(
        switchMap(
          result => this.auth.logged$,
          (result, isLogin) => {
            const randomUser = new Array(5)
              .map(() => Math.ceil(88888 * Math.random() + 888))
              .slice(0, result.totalCount);
            if (!isLogin || !result.donators) {
              result.donators = randomUser;
            }
            return result;
          },
        ),
      )
      .subscribe(result => {
        this.donors = result.donators;
        this.total = result.totalCount;
      });
  }
}
