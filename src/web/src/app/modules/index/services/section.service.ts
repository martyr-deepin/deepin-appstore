import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { environment } from 'environments/environment';
import { map } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class SectionService {
  url = `${environment.operationServer}/api/blob/section`;
  list = this.http
    .get<Section[]>(this.url)
    .pipe(
      map(ss => ss || []),
      map(ss => ss.filter(s => s.show)),
    )
    .toPromise();
  constructor(private http: HttpClient) {}
  getList() {
    return this.list;
  }
}

export enum SectionType {
  Carousel,
  Cover,
  Icon,
  Phrase,
  Ranking,
  Assemble,
  Topic,
}
export const SectionTypeString = {
  [SectionType.Carousel]: '轮播图',
  [SectionType.Cover]: '大图展示',
  [SectionType.Icon]: '小图标展示',
  [SectionType.Phrase]: '装机必备',
  [SectionType.Ranking]: '排行展示',
  [SectionType.Assemble]: '大小图组合竖排展示',
  [SectionType.Topic]: '专题展示',
};
export class SectionRanking {
  category = '';
  count = 10;
}
export class Section {
  type: SectionType = SectionType.Carousel;
  title: string[] = ['新建栏目', ''];
  colSpan = 1;
  rowSpan = 1;
  show = true;
  more = true;
  items = [];
  ranking: SectionRanking = new SectionRanking();
}

export class SectionApp {
  name = '';
  show = true;
}

export enum CarouselType {
  App,
  Topic = 1,
}
export class SectionCarousel {
  type = CarouselType.App;
  link = '';
  images = [];
  show = true;
}

export class SectionPhrase extends SectionApp {
  phrases: string[] = ['', ''];
}

export class SectionTopic {
  name: string[] = ['', ''];
  show = true;
  cover = '';
  coverHD = '';
  backgroundImage = '';
  backgroundImageHD = '';
  backgroundColor = '';
  nameColor = '';
  subTitleColor: '';
  apps: SectionApp[] = [];
}

export class SectionAssemble {
  category = '';
  show = true;
  apps: SectionApp[] = [];
}
