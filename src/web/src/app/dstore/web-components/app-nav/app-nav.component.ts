import { Component, OnInit, Input } from '@angular/core';

@Component({
  selector: 'app-nav',
  templateUrl: './app-nav.component.html',
  styleUrls: ['./app-nav.component.scss'],
})
export class AppNavComponent implements OnInit {
  @Input() nav: Nav[];

  constructor() {}

  ngOnInit() {}

  navURL(index: number): string[] {
    return [].concat.apply([], this.nav.slice(0, index + 1).map(n => n.url));
  }
}

export class Nav {
  title: string;
  url: string[];
}
