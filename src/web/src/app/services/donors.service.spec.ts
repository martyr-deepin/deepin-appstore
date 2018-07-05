import { TestBed, inject } from '@angular/core/testing';

import { DonorsService } from './donors.service';

describe('DonorsService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [DonorsService]
    });
  });

  it('should be created', inject([DonorsService], (service: DonorsService) => {
    expect(service).toBeTruthy();
  }));
});
