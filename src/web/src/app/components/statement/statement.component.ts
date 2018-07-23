import { Component, OnInit, ViewEncapsulation, ViewChild, ElementRef } from '@angular/core';
import { DstoreObject } from '../../dstore-client.module/utils/dstore-objects';
import { StatementService } from '../../services/statement.service';

@Component({
  selector: 'app-statement',
  templateUrl: './statement.component.html',
  styleUrls: ['./statement.component.scss'],
  encapsulation: ViewEncapsulation.Native,
})
export class StatementComponent implements OnInit {
  constructor(private statementService: StatementService) {}
  @ViewChild('container') container: ElementRef<HTMLDivElement>;

  statement: string;

  ngOnInit() {
    this.statementService.getStatement().subscribe(html => {
      this.statement = html;
      setTimeout(() => {
        Array.from(this.container.nativeElement.querySelectorAll<HTMLLinkElement>('a')).forEach(
          a => {
            a.addEventListener('click', e => {
              this.open(a.href);
              e.preventDefault();
            });
          },
        );
      });
    });
  }

  open(url: string) {
    DstoreObject.openURL(url);
  }
}
