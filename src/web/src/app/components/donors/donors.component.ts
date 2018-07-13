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
  @Input() random: number;
  donors = [];
  total = 0;
  ngOnInit() {}
  ngOnChanges() {
    this.donorsService.getDonation(this.appName).subscribe(result => {
      if (!result.donators) {
        result.donators = [];
      }
      while (result.donators.length < result.totalCount && result.donators.length < 5) {
        result.donators.push(Math.ceil(88888 * Math.random() + 888));
      }
      this.donors = result.donators;
      this.total = result.totalCount;
    });
  }
}
