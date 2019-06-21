import { environment } from 'environments/environment';

export class DstoreApp {
  name = '';
  LocalName = '';
  icon = '';
  cover = '';

  constructor(private app: RawApp) {
    this.name = app.name;
    if (app.info) {
      this.icon = environment.metadataServer + '/images/' + app.info.icon;
      this.LocalName = app.description.sort((a, b) => {
        return descriptionEvaluate(b) - descriptionEvaluate(a);
      })[0].name;
    }
    if (app.images) {
      this.cover = app.images
        .filter(img => [ImgType.ImgCover, ImgType.ImgCoverHD].includes(img.type))
        .sort((a, b) => {
          return imageEvaluate(b) - imageEvaluate(a);
        })[0].path;
    }
  }
  AttachTo<T extends Object>(obj: T): T & AttachApp {
    obj['dstoreApp'] = this;
    return obj as T & AttachApp;
  }
}
// 评估图片优先级
function imageEvaluate(img: Image): number {
  let value = 0;
  if (img.path !== '') {
    value += 100;
  }
  value += localeEvaluate(img.locale);
  if (devicePixelRatio > 1) {
    value += img.type;
  } else {
    value -= img.type;
  }
  return value;
}
// 评估应用详情优先级
function descriptionEvaluate(desc: Description): number {
  let value = 0;
  if (desc.name !== '') {
    value += 100;
  }
  value += localeEvaluate(desc.locale);
  return value;
}
// 评估不同语言优先级
function localeEvaluate(locale: string, list = [...navigator.languages]): number {
  const index = list.indexOf(locale.replace('_', '-'));
  if (index === -1) {
    return 0;
  }
  return list.length - index;
}

interface AttachApp {
  dstoreApp: DstoreApp;
}

export interface RawApp {
  id: number;
  updateAt: string;
  name: string;
  info: Info;
  description: Description[];
  images?: Image[];
}

interface Image {
  locale: string;
  path: string;
  type: number;
  order: number;
}

enum ImgType {
  ImgIcon = 1,
  ImgCover,
  ImgCoverHD,
  ImgScreenshot,
  ImgScreenshotHD,
}

interface Description {
  locale: string;
  name: string;
  description: string;
  slogan: string;
}

interface Info {
  author: string;
  packager: string;
  category: string;
  homePage: string;
  icon: string;
  packageURI: string;
  source: number;
  extra: string;
}
