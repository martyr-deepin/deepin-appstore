import { TestBed, inject } from '@angular/core/testing';

import { ReminderService } from './reminder.service';

describe('ReminderService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [ReminderService]
    });
  });

  it('should be created', inject([ReminderService], (service: ReminderService) => {
    expect(service).toBeTruthy();
  }));
});
