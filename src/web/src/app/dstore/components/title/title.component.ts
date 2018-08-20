import { Component, OnInit, Input } from '@angular/core';
import { UrlTree, Router, ActivatedRoute } from '@angular/router';

@Component({
  selector: 'dstore-title',
  templateUrl: './title.component.html',
  styleUrls: ['./title.component.scss'],
})
export class TitleComponent implements OnInit {
  @Input() title: string;
  @Input() more: any;

  constructor(private router: Router, private route: ActivatedRoute) {}

  ngOnInit() {}
}
