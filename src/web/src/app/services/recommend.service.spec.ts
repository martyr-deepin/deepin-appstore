import { TestBed, inject } from '@angular/core/testing';

import { RecommendService } from './recommend.service';

describe('RecommendService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [RecommendService]
    });
  });

  it('should be created', inject([RecommendService], (service: RecommendService) => {
    expect(service).toBeTruthy();
  }));
});
