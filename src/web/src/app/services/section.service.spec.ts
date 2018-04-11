import { TestBed, inject } from '@angular/core/testing';

import { SectionService } from './section.service';

describe('SectionService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [SectionService]
    });
  });

  it('should be created', inject([SectionService], (service: SectionService) => {
    expect(service).toBeTruthy();
  }));
});
