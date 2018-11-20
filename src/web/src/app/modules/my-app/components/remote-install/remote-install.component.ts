import { Component, OnInit, Input } from '@angular/core';
import { App } from 'app/services/app.service';

@Component({
  selector: 'dstore-remote-install',
  templateUrl: './remote-install.component.html',
  styleUrls: ['./remote-install.component.scss'],
})
export class RemoteInstallComponent implements OnInit {
  @Input()
  apps: App[] = [];
  constructor() {}

  ngOnInit() {}
}
