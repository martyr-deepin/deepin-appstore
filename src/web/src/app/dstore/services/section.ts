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

export class SectionRanking {
  category = '';
  count = 10;
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

export function sectionAddItem(section: Section, ...arge: any[]) {
  const newSectionItem = {
    [SectionType.Carousel]: new SectionCarousel(),
    [SectionType.Cover]: new SectionApp(),
    [SectionType.Icon]: new SectionApp(),
    [SectionType.Phrase]: new SectionPhrase(),
    [SectionType.Assemble]: new SectionAssemble(),
    [SectionType.Topic]: new SectionTopic(),
  };
  if (newSectionItem[section.type]) {
    section.items = [...section.items, newSectionItem[section.type]];
  }
}
