import {
  Component,
  OnInit,
  Input,
  ElementRef,
  OnChanges,
  SimpleChanges,
  ViewChild,
  Output,
  EventEmitter,
} from '@angular/core';

@Component({
  selector: 'dstore-list',
  templateUrl: './list.component.html',
  styleUrls: ['./list.component.scss'],
})
export class ListComponent implements OnInit, OnChanges {
  constructor() {}
  @ViewChild('loadingRef', { static: true }) elRef: ElementRef<HTMLDivElement>;
  @Input() list: [];
  @Input() lazyload = false;
  @Input() nameColor: string;
  @Input() subtitleColor: string;
  @Input() slogan = false;
  @Output() load = new EventEmitter<void>();
  wait = false;

  // 监听是否到达底部
  intersection = new IntersectionObserver(([e]: IntersectionObserverEntry[]) => {
    if (e.isIntersecting) {
      console.log('load');
      this.wait = true;
      this.load.next();
      this.intersection.unobserve(this.elRef.nativeElement);
    }
  });

  ngOnInit() {}

  ngOnChanges(changed: SimpleChanges) {
    if (changed.list) {
      this.wait = false;
      if (
        changed.list.previousValue &&
        changed.list.currentValue &&
        changed.list.previousValue.length === changed.list.currentValue.length
      ) {
        console.log('loaded');
        return;
      }
      if (this.lazyload) {
        console.log('loading');
        setTimeout(() => this.intersection.observe(this.elRef.nativeElement), 500);
      }
    }
  }
}
