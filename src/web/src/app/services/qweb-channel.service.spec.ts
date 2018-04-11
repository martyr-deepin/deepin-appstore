import { TestBed, inject } from '@angular/core/testing';

import { QwebChannelService } from './qweb-channel.service';

describe('QwebChannelService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [QwebChannelService]
    });
  });

  it('should be created', inject([QwebChannelService], (service: QwebChannelService) => {
    expect(service).toBeTruthy();
  }));
});
