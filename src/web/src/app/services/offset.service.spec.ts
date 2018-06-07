import { TestBed, inject } from '@angular/core/testing';

import { OffsetService } from './offset.service';

describe('OffsetService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [OffsetService]
    });
  });

  it('should be created', inject([OffsetService], (service: OffsetService) => {
    expect(service).toBeTruthy();
  }));
});
