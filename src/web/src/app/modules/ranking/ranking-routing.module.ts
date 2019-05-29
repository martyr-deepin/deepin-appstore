import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { RankingComponent } from './components/ranking/ranking.component';
import { RankingService } from './ranking.service';

const routes: Routes = [
  {
    path: '',
    component: RankingComponent,
    resolve: { data: RankingService },
    runGuardsAndResolvers: 'paramsOrQueryParamsChange',
  },
  {
    path: ':appName',
    loadChildren: 'app/modules/details/details.module#DetailsModule',
  },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class RankingRoutingModule {}
