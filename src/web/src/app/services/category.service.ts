import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from 'environments/environment';
import { Observable } from 'rxjs';
import { map, retry, shareReplay } from 'rxjs/operators';

@Injectable({
  providedIn: 'root',
})
export class CategoryService {
  private server = environment.operationServer;
  private apiURL = this.server + '/api/blob/category';
  private category$: Observable<Category[]>;

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
