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
  constructor(private route: ActivatedRoute, private router: Router) {}
  @Input() checkNet = true;
  @Input() timeout = 15;
  timeout$: Observable<string>;
  waitOnline$: Observable<void>;
  online: boolean;
  ngOnInit() {
    if (this.checkNet) {
      this.online = navigator.onLine;
    } else {
      this.online = true;
    }
    console.log(this.checkNet, this.online, navigator.onLine);
    this.timeout$ = timer(this.online ? this.timeout * 1000 : 0).pipe(map(() => 'timeout'));
    this.waitOnline$ = fromEvent(window, 'online').pipe(
      map(() => {
        this.refresh();
      }),
    );
  }
  refresh() {
    location.reload();
  }
}
