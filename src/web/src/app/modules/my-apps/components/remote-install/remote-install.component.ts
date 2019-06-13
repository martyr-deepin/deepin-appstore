import { Component, OnInit, Input } from '@angular/core';
import { AutoInstallService } from '../../services/auto-install.service';

@Component({
  selector: 'dstore-remote-install',
  templateUrl: './remote-install.component.html',
  styleUrls: ['./remote-install.component.scss'],
})
export class RemoteInstallComponent implements OnInit {
  constructor(private autoInstallService: AutoInstallService) {}
  autoInstall = this.autoInstallService.getAutoInstall();
  ngOnInit() {}
  change(auto: boolean) {
    this.autoInstallService.setAutoInstall(auto);
  }
}
