import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { environment } from 'environments/environment';
import { map, tap, switchMap } from 'rxjs/operators';
import { StoreService } from 'app/modules/client/services/store.service';
import { Category, CategoryService } from './category.service';

@Injectable({
  providedIn: 'root',
})
export class SoftwareService {
  constructor(
    private http: HttpClient,
    private categoryService: CategoryService,
    private storeService: StoreService,
  ) {}
  readonly url = environment.metadataServer + '/api/v3/apps';

  getSofts(names: string[]) {
    console.log('getSofts');
    const preloads = ['info', 'desc', 'tags', 'images'];
    const params = { names: [...names].sort(), preloads };
    return this.http.get<Software[]>(this.url, { params }).pipe(
      map(softs =>
        softs
          .map(this.convertInfo)
          .sort((a, b) => names.indexOf(a.name) - names.indexOf(b.name)),
      ),
      tap(softs => {
        console.log('softs', softs);
        softs.forEach(soft => {
          delete soft.desc;
          delete soft.images;
          delete soft.tags;
          delete soft.versions;
          delete soft.info.packageURI;
        });
      }),
    );
  }

  attach<T extends object>(soft: Software, obj: T) {
    return { ...soft, ...(obj as object) } as Software & T;
  }

  convertInfo(soft: Software): Software {
    soft.info.packages = JSON.parse(soft.info.packageURI || '[]').map(url => ({
      packageURI: url,
    }));
    soft.info.extra = JSON.parse((soft.info.extra as any) || '{}');
    // locale desc
    soft.info = {
      ...soft.info,
      ...((soft.desc as any) || [])
        .sort(sortByLocale)
        .filter(desc => desc.name)[0],
    };
    // locale tags
    soft.info.tags = localeFilter(soft.tags || []).map(tags => tags.tag);
    soft.images = soft.images || [];
    // locale cover
    for (const ratio of devicePixelRatio === 1 ? [0, 1] : [1, 0]) {
      const cover = soft.images
        .filter(img => img.type === ImageType.Cover + ratio)
        .sort(sortByLocale)[0];
      if (cover) {
        soft.info.cover = cover.path;
        break;
      }
    }
    // locale screenshot
    for (const ratio of devicePixelRatio === 1 ? [0, 1] : [1, 0]) {
      const screenshot = soft.images.filter(
        img => img.type === ImageType.Screenshot + ratio,
      );
      const groupByLocale = screenshot.reduce((acc, value) => {
        const v = acc.find(v => v.locale === value.locale);
        if (v) {
          v.imgs.push(value);
        } else {
          acc.push({ locale: value.locale, imgs: [value] });
        }
        return acc;
      }, []);
      if (groupByLocale.length) {
        soft.info.screenshot = groupByLocale
          .sort(sortByLocale)[0]
          .imgs.sort((a, b) => a.order - b.order)
          .map(img => img.path);
      }
    }
    return soft;
  }
  private toQuery(soft: Software) {
    return {
      name: soft.name,
      localName: soft.info.name,
      packages: soft.info.packages,
    };
  }
  open(soft: Software) {
    return this.storeService.execWithCallback(
      'storeDaemon.openApp',
      this.toQuery(soft),
    );
  }
  install(...softs: Software[]) {
    return this.storeService.execWithCallback(
      'storeDaemon.installPackages',
      softs.map(this.toQuery),
    );
  }
}

interface Locale {
  locale: string;
}

function sortByLocale(a: Locale, b: Locale) {
  const level = [environment.locale, 'en_US', 'zh_CN'];
  return level.indexOf(a.locale) - level.indexOf(b.locale);
}

function localeFilter(arr: Locale[]): any[] {
  if (arr.some(v => v.locale === environment.locale)) {
    return arr.filter(v => v.locale === environment.locale);
  }
  return arr.filter(v => v.locale === 'en_US');
}

// 软件信息
interface Info extends Desc {
  author: string;
  packager: string;
  category: string;
  homePage: string;
  icon: string;
  packages: { packageURI: string }[];
  packageURI: string;
  source: number;
  extra: {};
  versions: any[];
  tags: any[];
  cover: string;
  screenshot: string[];
}
export interface Software {
  id: number;
  created_at: string;
  updated_at: string;
  name: string;
  info: Info;
  // 下面是服务器返回结构，全部解析到info内部
  desc: Desc;
  versions?: any;
  tags: any[];
  images: Image[];
}

interface Desc {
  locale: string;
  name: string;
  description: string;
  slogan: string;
}

interface Image {
  locale: string;
  path: string;
  type: number;
  order: number;
}
export enum ImageType {
  Invalid,
  Icon,
  Cover,
  CoverHD,
  Screenshot,
  ScreenshotHD,
}
