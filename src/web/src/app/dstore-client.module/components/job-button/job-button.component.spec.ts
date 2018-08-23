import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { JobButtonComponent } from './job-button.component';

describe('JobButtonComponent', () => {
  let component: JobButtonComponent;
  let fixture: ComponentFixture<JobButtonComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ JobButtonComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(JobButtonComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
