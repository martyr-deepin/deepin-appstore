import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { StoreJobErrorComponent } from './store-job-error.component';

describe('StoreJobErrorComponent', () => {
  let component: StoreJobErrorComponent;
  let fixture: ComponentFixture<StoreJobErrorComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ StoreJobErrorComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(StoreJobErrorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
