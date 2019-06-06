import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { environment } from 'environments/environment';
import { map, tap, switchMap, first } from 'rxjs/operators';
import { StoreService } from 'app/modules/client/services/store.service';
import { Category, CategoryService } from './category.service';
import { PackageService } from './package.service';
import { BaseService } from 'app/dstore/services/base.service';

@Injectable({
  providedIn: 'root',
})
export class SoftwareService {
  constructor(
    private http: HttpClient,
    private categoryService: CategoryService,
    private storeService: StoreService,
    private packageService: PackageService,
  ) {}
  private readonly native = BaseService.isNative;
  private readonly metadataURL = environment.metadataServer + '/api/v3/apps';
  // operation app url
  private readonly operationURL = environment.operationServer + '/api/v3/apps';

  async list({
    order = 'download' as 'download' | 'score',
    offset = 0,
    limit = 20,
    category = '',
    tag = '',
    names = [],
    filter = true,
  }) {
    if (names.length) {
      const stats = await this.http.get<Stat[]>(this.operationURL, { params: { names } }).toPromise();
      if (stats.length === 0) {
        return [];
      }
      const softs = await this.getSofts(stats.map(stat => stat.name)).toPromise();

      if (filter && this.native) {
        const packages = await this.packageService.querys(softs.map(this.toQuery));
        names = names.filter(name => packages.find(pkg => name === pkg.appName));
      }

      return names
        .map(name => {
          const stat = stats.find(stat => stat.name === name);
          const soft = softs.find(soft => soft.name === name);
          if (soft) {
            soft.stat = stat;
          }
          return soft;
        })
        .filter(soft => soft && soft.stat);
    }
    let stats = await this.http
      .get<Stat[]>(this.operationURL, {
        params: { order, offset, limit, category, tag } as any,
      })
      .toPromise();
    if (stats.length === 0) {
      return [];
    }
    const softs = await this.getSofts(stats.map(v => v.name)).toPromise();

    if (filter && this.native) {
      const packages = await this.packageService.querys(softs.map(this.toQuery));
      stats = stats.filter(stat => packages.find(pkg => stat.name === pkg.appName));
    }

    return stats
      .map(stat => {
        const soft = softs.find(s => s.name === stat.name);
        if (soft) {
          soft.stat = stat;
        }
        return soft;
      })
      .filter(Boolean);
  }

  private getSofts(names: string[]) {
    const preloads = ['info', 'desc', 'tags', 'images'];
    const params = { names: [...names].sort(), preloads };
    return this.http.get<Software[]>(this.metadataURL, { params }).pipe(
      map(softs => softs.map(this.convertInfo).sort((a, b) => names.indexOf(a.name) - names.indexOf(b.name))),
      tap(softs => {
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

  // app json data convert
  private convertInfo(soft: Software): Software {
    soft.info.packages = JSON.parse(soft.info.packageURI || '[]').map(url => ({
      packageURI: url,
    }));
    soft.info.extra = JSON.parse((soft.info.extra as any) || '{}');
    // locale desc
    soft.info = {
      ...soft.info,
      ...((soft.desc as any) || []).sort(sortByLocale).filter(desc => desc.name)[0],
    };
    // locale tags
    soft.info.tags = localeFilter(soft.tags || []).map(tags => tags.tag);
    soft.images = soft.images || [];

    // icon
    soft.info.icon = environment.metadataServer + '/images/' + soft.info.icon;
    // locale cover
    for (const ratio of devicePixelRatio === 1 ? [0, 1] : [1, 0]) {
      const cover = soft.images.filter(img => img.type === ImageType.Cover + ratio).sort(sortByLocale)[0];
      if (cover) {
        soft.info.cover = environment.metadataServer + '/images/' + cover.path;
        break;
      }
    }
    // locale screenshot
    for (const ratio of devicePixelRatio === 1 ? [0, 1] : [1, 0]) {
      const screenshot = soft.images.filter(img => img.type === ImageType.Screenshot + ratio);
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

  // software convert to package query
  private toQuery(soft: Software) {
    return {
      name: soft.name,
      localName: soft.info.name,
      packages: soft.info.packages,
    };
  }

  // open software
  open(soft: Software) {
    return this.storeService.execWithCallback('storeDaemon.openApp', this.toQuery(soft));
  }
  // install software
  install(...softs: Software[]) {
    return this.storeService.execWithCallback('storeDaemon.installPackages', softs.map(this.toQuery));
  }
}

interface Locale {
  locale: string;
}

export function sortByLocale(a: Locale, b: Locale) {
  const level = [environment.locale, 'en_US', 'zh_CN'];
  return level.indexOf(a.locale) - level.indexOf(b.locale);
}

export function localeFilter(arr: Locale[]): any[] {
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
  stat: Stat;
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
export interface Stat {
  name: string;
  score: number;
  score_count: number;
  download: number;
}
