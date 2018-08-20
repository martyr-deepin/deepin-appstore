import { Component, OnInit, Output } from '@angular/core';
import { FormControl } from '@angular/forms';
import { debounceTime } from 'rxjs/operators';

@Component({
  selector: 'dstore-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.scss'],
})
export class SearchComponent implements OnInit {
  constructor() {}
  search = new FormControl();
  @Output() valueChanges = this.search.valueChanges.pipe(debounceTime(500));
  ngOnInit() {
    console.log(this.search);
  }
}
