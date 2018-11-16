import { Component, OnInit, Input } from '@angular/core';
import { Observable, timer, fromEvent } from 'rxjs';
import { map } from 'rxjs/operators';
import { ActivatedRoute, Router } from '@angular/router';

@Component({
  selector: 'app-wait',
  templateUrl: './wait.component.html',
  styleUrls: ['./wait.component.scss'],
})
export class WaitComponent implements OnInit {
  constructor(private route: ActivatedRoute) {}
  @Input() timeout = 15;
  @Input() delay = 0;
  timeout$: Observable<boolean>;
  delay$: Observable<boolean>;
  ngOnInit() {
    this.timeout$ = timer(this.timeout * 1000).pipe(map(() => true));
    this.delay$ = timer(this.delay * 1000).pipe(map(() => true));
  }
}
