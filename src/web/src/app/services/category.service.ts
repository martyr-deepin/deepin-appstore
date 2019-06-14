import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { map, retry, shareReplay } from 'rxjs/operators';

import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class CategoryService {
  private server = environment.operationServer;
  private apiURL = this.server + '/api/blob/category';
  private category$: Observable<Category[]>;
  metadataCategory$ = this.getMetadataCategory();

  constructor(private http: HttpClient) {
    this.category$ = this.http.get<CustomCategory[]>(this.apiURL).pipe(
      map(ccs => {
        if (ccs) {
          return ccs
            .filter(c => c.show)
            .map((c, index) => ({
              id: index.toString(),
              title: c.name,
              icon: c.icon.map(i => this.server + '/images/' + i),
              apps: c.apps,
            }));
        } else {
          return [];
        }
      }),
      shareReplay(),
    );
  }

  private async getMetadataCategory() {
    let categorys = await this.http.get<MetadataCategory[]>(environment.metadataServer + '/api/category').toPromise();
    if (categorys.some(c => c.Locale === environment.locale)) {
      categorys = categorys.filter(c => c.Locale === environment.locale);
    } else {
      categorys = categorys.filter(c => c.Locale === 'en_US');
    }
    return new Map(categorys.map(c => [c.Name, c.LocalName]));
  }
  operationCategory() {
    this.http.get(environment.operationServer + '/api/blob/category');
  }

  list() {
    return this.category$;
  }
}

export interface Category {
  id: string;
  title: string;
  icon: string[];
  apps?: string[];
}

interface CustomCategory {
  name: '';
  icon: string[];
  show: boolean;
  apps: string[];
}
interface MetadataCategory {
  Active: boolean;
  CreatedAt: string;
  DeletedAt?: any;
  ID: number;
  Icon: string;
  IconActive: string;
  LocalName: string;
  Locale: string;
  Name: string;
  Priority: number;
  UpdatedAt: string;
}
