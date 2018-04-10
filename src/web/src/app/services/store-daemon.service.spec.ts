import { TestBed, inject } from '@angular/core/testing';

import { StoreDaemonService } from './store-daemon.service';

describe('StoreDaemonService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [StoreDaemonService]
    });
  });

  it('should be created', inject([StoreDaemonService], (service: StoreDaemonService) => {
    expect(service).toBeTruthy();
  }));
});
