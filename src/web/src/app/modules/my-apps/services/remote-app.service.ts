import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'environments/environment';

@Injectable({
  providedIn: 'root',
})
export class RemoteAppService {
  url = environment.operationServer + '/api/my/app';
  constructor(private http: HttpClient) {}
  RemoteAppList(page: number, pageSize: number) {
    const params = {
      page: page.toString(),
      count: pageSize.toString(),
    };
    return this.http.get<Result>(this.url, { params });
  }
}

interface Result {
  apps: RemoteApp[];
  totalCount: number;
}
interface RemoteApp {
  appName: string;
  time: Date;
}
