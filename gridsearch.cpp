/*
* @Author: shayaan
* @Date:   2020-06-10 13:24:55
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-11 10:06:00
*/

#include <bits/stdc++.h>
#include "parSet.h"
#include "sanityCheckOnNetPosAcct.cpp"
#include "criterion.cpp"

static parSet gridSearch(int nAccts, double cumPnl, double netPosSystem, vector<double> netPosAcct, vector<double> af, 
						int qt, vector<double> pnlAcctSoFar, double pricePre, double priceNow, int onePtValue, int scale,
						char criteriaOption, int sanityOnNetPosition){

	parSet p(nAccts,af,onePtValue);

	int sgnNet;
	if(netPosSystem > 0){
		sgnNet = 1; 
	}
		
	if(netPosSystem < 0){
		sgnNet = -1; 
	}
	
	vector <double> par;
	par.clear();
	for (int i = 0; i < af.size(); i++){
		par.push_back(round (af[i] * fabs( netPosSystem )));
	}
	

	while ( accumulate(begin(par), end(par), 0) != fabs(netPosSystem)){
		if (accumulate(begin(par), end(par), 0) > fabs(netPosSystem)){
			int max = par[0];
			vector<int> maxIndex = {0};
			for(int i = 0; i < par.size(); i++){
				if (par[i] > max){
					max = par[i];
					maxIndex.clear();
					maxIndex.push_back(i);
				}
				if (par[i] == max){
					maxIndex.push_back(i);
				}
			}

			int INDEX;
			if(maxIndex.size() > 1){
				int minaf = 1;
				for(int i = 0; i < maxIndex.size(); i++){
					if(af[maxIndex[i]] < minaf){
						minaf = af[maxIndex[i]];
						INDEX = i;
					}
				}
			} 

			else{
				INDEX = maxIndex[0];
			}
			par[INDEX] -= 1;

		}

		else{
			int max = par[0];
			vector<int> maxIndex = {0};
			for(int i = 0; i < par.size(); i++){
				if (par[i] > max){
					max = par[i];
					maxIndex.clear();
					maxIndex.push_back(i);
				}
				if (par[i] == max){
					maxIndex.push_back(i);
				}
			}
			int INDEX;
			if(maxIndex.size() > 1){
				int minaf = 1;

				for(int i = 0; i < maxIndex.size(); i++){
					if(af[maxIndex[i]] < minaf){
						minaf = af[maxIndex[i]];
						INDEX = i;
					}
				}
			} else{
				INDEX = maxIndex[0];
			}
			par[INDEX] += 1;

		}
	}

	vector<double> netPosAcct2;
	netPosAcct2.clear();
	for (int i = 0; i < par.size(); i++){
		netPosAcct2.push_back(sgnNet*par[i]);
	}

	vector<double> qtAcct;
	qtAcct.clear();


	for(int i = 0; i < netPosAcct2.size(); i++){
		qtAcct.push_back(netPosAcct2[i] - netPosAcct[i]);
	}

	for(int i = 0; i < qtAcct.size(); i++){
		p.optimalAllocation[i] = fabs(qtAcct[i]);
	}

	
	netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, scale, qt);
	qtAcct.clear();
	

	for(int i = 0; i < netPosAcct2.size(); i++){
		qtAcct.push_back(netPosAcct2[i] - netPosAcct[i]);
	}

	for(int i = 0; i < qtAcct.size(); i++){
		p.optimalAllocation[i] = fabs(qtAcct[i]);
	}


	double sumqtAcct = 0;

	for(int i = 0; i < qtAcct.size(); i++){
		sumqtAcct += qtAcct[i];
	}

	if((scale < 0 && sumqtAcct > 0)>0 || (scale > 0 && sumqtAcct < 0) > 0 || sumqtAcct != scale * qt){
		cout << "ERROR: In gridSreach2 - something is not right ..." << endl;
	}



	p.netPosAcct2 = netPosAcct2;
	p.sgnNet = sgnNet;
	p.af = af;
	p.cumPnl = cumPnl;
	p.pnlAcctSoFar = pnlAcctSoFar;
	p.netPosAcct = netPosAcct;
	p.scale = scale;
	p.qt = qt;
	p.pricePre = pricePre;
	p.priceNow = priceNow;
	p.onePtValue = onePtValue;
	p.criteriaOption = criteriaOption;
	p.sanityOnNetPosition = sanityOnNetPosition;

	vector <double> pnlAcctSoFar2;
	pnlAcctSoFar2.clear();

	for (int i = 0; i < pnlAcctSoFar.size(); i++ ){
		pnlAcctSoFar2.push_back( pnlAcctSoFar[i] + netPosAcct2[i] * (priceNow - pricePre) * onePtValue); // PnL with new positions

	}



	p.minMae = criterion(pnlAcctSoFar2, cumPnl, af, 1);

	p.optimalCase.clear();
	p.netSoFar = netPosAcct2;

	p.perturbVec = {1, -1};
	p.optimizingPnlByPerturbingNetPos();


	// p.perturbVec = {1, -1, 1, -1};
	// p.optimizingPnlByPerturbingNetPos();

	// p.perturbVec = {-1, -1, 2};
	// p.optimizingPnlByPerturbingNetPos();

	// p.perturbVec = {1, 1, -2};
	// p.optimizingPnlByPerturbingNetPos();

	// p.perturbVec = {2,-2};
	// p.optimizingPnlByPerturbingNetPos();

	// p.perturbVec = {3, -3};
	// p.optimizingPnlByPerturbingNetPos();

	// p.perturbVec = {1, 2, -3};
	// p.optimizingPnlByPerturbingNetPos();

	// p.perturbVec = {3, -1, -2};
	// p.optimizingPnlByPerturbingNetPos();
	
	return p;
}

