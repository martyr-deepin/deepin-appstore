import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { DonorsComponent } from './donors.component';

describe('DonorsComponent', () => {
  let component: DonorsComponent;
  let fixture: ComponentFixture<DonorsComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ DonorsComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(DonorsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
