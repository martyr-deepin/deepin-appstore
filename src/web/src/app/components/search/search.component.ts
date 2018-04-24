import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Observable } from 'rxjs/Observable';
import { App } from '../../services/app.service';

@Component({
  selector: 'app-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.scss'],
})
export class SearchComponent implements OnInit {
  constructor(private route: ActivatedRoute) {}

  apps$: Observable<App>;
  ngOnInit() {
    this.route.paramMap.subscribe(param => {
      const apps = param.getAll('apps');
    });
  }
}
