import { Component, OnInit, Input } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Router, ActivatedRoute, NavigationExtras } from '@angular/router';
import 'rxjs/add/operator/debounceTime';

@Component({
  selector: 'app-search',
  templateUrl: './app-search.component.html',
  styleUrls: ['./app-search.component.scss']
})
export class AppSearchComponent implements OnInit {
  searchControl = new FormControl();

  constructor(private router: Router, private route: ActivatedRoute) {}

  ngOnInit() {
    this.route.queryParamMap.subscribe(query =>
      this.searchControl.setValue(query.get('search'))
    );
    this.searchControl.valueChanges
      .debounceTime(300)
      .subscribe(this.onSearch.bind(this));
  }

  onSearch(search?: string) {
    const extras: NavigationExtras = {
      relativeTo: this.route
    };
    if (search) {
      extras.queryParams = { search };
    }
    this.router.navigate(['../1'], extras);
  }
}
