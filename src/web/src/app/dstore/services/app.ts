import * as _ from 'lodash';
import { AppVersion } from '../../dstore-client.module/models/app-version';

export class App {
  id = 0;
  userID: number;
  submitterEmail: string;
  state: AppState;
  updateTime: Date;
  comment: string;
  name: string;
  author: string;
  packager: string;
  category: string;
  homePage: string;
  icon: string;
  packageURI: string[];
  extra: { [key: string]: any };
  locale: { [key: string]: LocalInfo };
  constructor() {
    const model = {
      name: '',
      author: '',
      packager: '',
      category: '',
      homePage: '',
      icon: '',
      packageURI: '',
      extra: '',
      locale: {
        en_US: new LocalInfo(),
        zh_CN: new LocalInfo(),
      },
    };
    return JSON.parse(JSON.stringify(model), appReviver);
  }

  localInfo: LocalInfo;
  localCategory: string;
  version: AppVersion;

  downloads: number;
}

// 自定义序列号App JSON
export function appReplacer(k: string, v: any): any {
  v = JSON.parse(JSON.stringify(v));
  switch (k) {
    case 'locale':
      return _.pickBy(<{ [key: string]: LocalInfo }>v, info => info.description.name);
    case 'packageURI':
      return JSON.stringify(v);
    case 'extra':
      return JSON.stringify(v);
    case 'versions':
      return (<Version[]>v).filter(vv => vv.version);

    case 'images':
      const img = <Images>v;
      return <Image[]>[
        {
          type: ImageType.Cover,
          path: img.cover,
        },
        {
          type: ImageType.CoverHD,
          path: img.coverHD,
        },
        ...img.screenshot.map((imgPath, index) => ({
          type: ImageType.Screenshot,
          path: imgPath,
          order: index,
        })),
        ...img.screenshotHD.map((imgPath, index) => ({
          type: ImageType.ScreenshotHD,
          path: imgPath,
          order: index,
        })),
      ].filter(image => image.path);
  }
  return v;
}
// 自定义反序列App JSON
export function appReviver(k: string, v: any): any {
  switch (k) {
    case 'updateTime':
      return new Date(v);
    case 'tags':
      return v || [];
    case 'versions':
      return v || [new Version()];
    case 'packageURI':
      return v ? JSON.parse(v) : [''];
    case 'extra':
      return v ? JSON.parse(v) : { open: 'desktop' };
    case 'locale':
      return {
        zh_CN: v['zh_CN'] || new LocalInfo(),
        en_US: v['en_US'] || new LocalInfo(),
      };
    case 'images':
      const images: Images = {
        cover: undefined,
        coverHD: undefined,
        screenshot: [],
        screenshotHD: [],
      };
      _.chain(<Image[]>v)
        .groupBy(img => img.type)
        .each((imgs, type) => {
          switch (+type) {
            case ImageType.Cover:
              images.cover = _.first(imgs).path;
              break;
            case ImageType.CoverHD:
              images.coverHD = _.first(imgs).path;
              break;
            case ImageType.Screenshot:
              images.screenshot = _
                .chain(imgs)
                .sortBy('order')
                .map('path')
                .value();
              break;
            case ImageType.ScreenshotHD:
              images.screenshotHD = _
                .chain(imgs)
                .sortBy('order')
                .map('path')
                .value();
              break;
          }
        })
        .value();
      return images;
    default:
      return v;
  }
}

export enum ErrCode {
  Unknown,
  NotFound,
  Existed,
}

export class Error {
  code: ErrCode;
  content: string;
}

export enum AppState {
  AppStateInvalid,
  AppStateActive,
  AppStateInactive,
  AppStateReviewInProcess,
  AppStateReviewDenied,
  AppStateDie,
}

export class LocalInfo {
  description: Description;
  images: Images;
  versions: Version[];
  tags: string[];
  constructor() {
    return {
      description: new Description(),
      images: new Images(),
      versions: [new Version()],
      tags: [],
    };
  }
}

class Description {
  name: string;
  description: string;
  slogan: string;
  constructor() {
    return {
      name: '',
      description: '',
      slogan: '',
    };
  }
}

export class Version {
  version: string;
  changeLog: string;
  isNew?: boolean;
  constructor() {
    return {
      version: '',
      changeLog: '',
      isNew: true,
    };
  }
}

export enum ImageType {
  Invalid,
  Icon,
  Cover,
  CoverHD,
  Screenshot,
  ScreenshotHD,
}

class Images {
  cover: string;
  coverHD: string;
  screenshot: string[];
  screenshotHD: string[];
  constructor() {
    return {
      cover: '',
      coverHD: '',
      screenshot: [],
      screenshotHD: [],
    };
  }
}
interface Image {
  path: string;
  type: ImageType;
  order?: number;
}

export function appSearch(app: App, search: string): boolean {
  const s = search.toLocaleLowerCase();
  return _
    .flatMap(app.locale, localInfo => Object.values(localInfo.description))
    .concat(app.name)
    .map((v: string) => v.toLocaleLowerCase().includes(s))
    .includes(true);
}

export function getAppLocalName(app: App): string {
  const localList = Object.keys(app.locale);
  if (localList.length === 0) {
    return '';
  }
  return app.locale[localList[localList.length - 1]].description.name;
}

export function appPaging(apps: App[], page: number, size = 10) {
  const chunkResult = _.chunk(apps, size);
  return {
    page,
    pageSize: size,
    pageCount: chunkResult.length,

    appCount: apps.length,
    appList: chunkResult[page - 1],
  };
}
export interface AppPageResult {
  appList: App[];
  appCount: number;
  page: number;
  pageCount: number;
  pageSize: number;
}
