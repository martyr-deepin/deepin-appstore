import { Component, OnInit, Input } from '@angular/core';
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
  @Input() checkNet = true;
  @Input() timeout = 10;
  timeout$: Observable<string>;
  online: boolean;
  ngOnInit() {
    if (this.checkNet) {
      this.online = navigator.onLine;
    } else {
      this.online = true;
    }
    this.timeout$ = timer(navigator.onLine ? this.timeout * 1000 : 0).pipe(map(() => 'timeout'));
  }
  refresh() {
    location.reload();
  }
}
