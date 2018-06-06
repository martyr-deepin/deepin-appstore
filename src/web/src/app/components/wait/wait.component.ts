import { Component, OnInit } from '@angular/core';
import { Observable, timer } from 'rxjs';
import { map } from 'rxjs/operators';
import { ActivatedRoute, Router } from '@angular/router';

@Component({
  selector: 'app-wait',
  templateUrl: './wait.component.html',
  styleUrls: ['./wait.component.scss'],
})
export class WaitComponent implements OnInit {
  constructor(private route: ActivatedRoute, private router: Router) {}
  online = navigator.onLine;
  timeout$ = timer(10e3).pipe(map(() => 'timeout'));
  ngOnInit() {}
  refresh() {
    location.reload();
  }
}
