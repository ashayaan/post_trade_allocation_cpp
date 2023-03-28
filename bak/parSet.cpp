/*
* @Author: shayaan
* @Date:   2020-06-11 15:58:49
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-11 23:23:07
*/

#include "allInclude.h"
#include "parSet.h"

parSet::parSet(int n,vector<double> allocation,int onePtValue){
	for(int i=0;i<n;i+=1){
		optimalAllocation.push_back(0);
		netPosAcct2.push_back(0);
		pnlAcctSoFar.push_back(0);
		netPosAcct.push_back(0);		
		netSoFar.push_back(0);
	}

	sgnNet = 1;
	af = allocation;
	cumPnl = 0;
	scale = 1;
	qt = 1;
	pricePre = 200;
	priceNow = 200;
	onePtValue = onePtValue;
	criteriaOption = '1';
	sanityOnNetPosition = 0;
	minMae = 0;
	perturbVec;
	optimalCase;

}


void parSet::optimizingPnlByPerturbingNetPos(){
	int nAccts = af.size();
	vector< double > par;
	par.clear();
	for (int i = 0; i < nAccts ; i++){
		par.push_back(fabs(netPosAcct2[i]));
	}
	vector < double > actionVector(nAccts, 0);
	vector < double > par1;

	//INSIDE VARIABLES
	vector <double> netPosAcct2_ins  = netPosAcct2;




	int flag = 0;
	if (perturbVec.size() == 2){

		int xi = perturbVec[0];
		int xj = perturbVec[1];
		for(int i = 0; i < nAccts; i++){
			for(int j = 0; j < nAccts; j++){
				if(i != j){
					// cout << "i: " << i << endl;
					// cout << "j: " << j << endl;
					fill(actionVector.begin(), actionVector.end(), 0);
					actionVector[i] = xi;
					actionVector[j] = xj;

					par1.clear();
					for(int p = 0; p <nAccts;p++){
						par1.push_back(par[p] + actionVector[p]);

					}

					int negFlag = 0;

					for (int p = 0; p < nAccts; p++){
						if(par1[p] < 0){ negFlag -= 1;}
					}

					if ( negFlag == 0){

						for (int p = 0; p < nAccts; p++){
							netPosAcct2_ins[p] = sgnNet * par1[p];
						}

						netPosAcct2_ins = sanityCheckOnNetPosAcct( netPosAcct, netPosAcct2_ins, scale, qt);
						vector < double > pnlAcctSoFar2;
						pnlAcctSoFar2.clear();

						for (int p = 0; p < nAccts; p++ ){
							pnlAcctSoFar2.push_back(pnlAcctSoFar[p] + netPosAcct2_ins[p] * (priceNow - pricePre) * onePtValue);
						}
						
						double mae = criterion(pnlAcctSoFar2, cumPnl, af, 1);

						if (mae == minMae){
							double e1, e2;
							vector <double > mid1, mid2;
							transform( netPosAcct2_ins.begin(), netPosAcct2_ins.end(), af.begin(), back_inserter(mid1), opDiv);
							e1 = entropy( mid1 );
							transform( netSoFar.begin(), netSoFar.end(), af.begin(), back_inserter(mid2), opDiv);
							e2 = entropy( mid2 );
							if(e1 < e2){
								flag = 1;
							}
						}


						if (mae < minMae || flag){
							minMae = mae;
							optimalCase.clear();
							optimalCase.push_back(i);
							optimalCase.push_back(j);
							optimalCase.push_back(actionVector[i]);
							optimalCase.push_back(actionVector[j]);
							vector<double> qtAcct;

							qtAcct.clear();
							for(int p = 0; p <netPosAcct2_ins.size(); p++){
								qtAcct.push_back(netPosAcct2_ins[p] - netPosAcct[p]);
							}
							fill(optimalAllocation.begin(), optimalAllocation.end(), 0);
							for (int p = 0; p < nAccts; p++){
								optimalAllocation[p] = fabs(qtAcct[p]);
							}
							netSoFar = netPosAcct2_ins;
							flag = 0;

						}

					}

				}
			}
		}
	}

	if (perturbVec.size() == 3 && nAccts >= 3){
		int xi = perturbVec[0];
		int xj = perturbVec[1];
		int xk = perturbVec[2];
		for (int i = 0; i < nAccts; i++){
			for (int j = 0; j < nAccts; j++){
				for (int k = 0; k < nAccts; k++){
					if (i != j && i != k && j!=k){
						// cout << "i: " << i << " j: " << j << " k: " << k << endl;


						fill(actionVector.begin(), actionVector.end(), 0);
						actionVector[i] = xi;
						actionVector[j] = xj;
						actionVector[k] = xk;
						par1.clear();
						for(int p = 0; p <nAccts;p++){
							par1.push_back(par[p] + actionVector[p]);

						}

						int negFlag = 0;
						for (int p = 0; p < nAccts; p++){
							if(par1[p] < 0){ negFlag -= 1;}
						}

						if(negFlag == 0){
							for (int p = 0; p < nAccts; p++){
								netPosAcct2_ins[p] = sgnNet * par1[p];
							}
							netPosAcct2_ins = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2_ins, scale, qt);
							vector < double > pnlAcctSoFar2;
							pnlAcctSoFar2.clear();
							for (int p = 0; p < nAccts; p++ ){
								pnlAcctSoFar2.push_back(pnlAcctSoFar[p] + netPosAcct2_ins[p] * (priceNow - pricePre) * onePtValue);
							}

							double mae = criterion(pnlAcctSoFar2, cumPnl, af, 1);
							if (mae == minMae){
								double e1, e2;
								vector <double > mid1, mid2;
								transform( netPosAcct2_ins.begin(), netPosAcct2_ins.end(), af.begin(), back_inserter(mid1), opDiv);
								e1 = entropy( mid1 );
								transform( netSoFar.begin(), netSoFar.end(), af.begin(), back_inserter(mid2), opDiv);
								e2 = entropy( mid2 );
								if(e1 < e2){
									flag = 1;
								}
							}

							if (mae < minMae || flag){
								minMae = mae;
								optimalCase.push_back(i);
								optimalCase.push_back(j);
								optimalCase.push_back(k);
								optimalCase.push_back(actionVector[i]);
								optimalCase.push_back(actionVector[j]);
								optimalCase.push_back(actionVector[k]);
								vector<double> qtAcct;
								qtAcct.clear();
								for(int p = 0; p <netPosAcct2_ins.size(); p++){
									qtAcct.push_back(netPosAcct2_ins[p] - netPosAcct[p]);
								}
								for (int p = 0; p < nAccts; p++){
									optimalAllocation[p] = fabs(qtAcct[p]);
								}
								netSoFar = netPosAcct2_ins;
								flag = 0;

							}
						}

					}
				}
			}
		}



	}

	if (perturbVec.size() == 4 && nAccts >= 4){
		int xi = perturbVec[0];
		int xj = perturbVec[1];
		int xk = perturbVec[2];
		int xl = perturbVec[3];
		for (int i = 0; i < nAccts; i++) {
			for (int j = 0; j < nAccts; j++) {
				for (int k = 0; k < nAccts; k++) {
					for (int l = 0; l < nAccts; l++){
						if (i != j && i != k && i!=l && j!=k && j!=l && k!=l) {
							// cout << "i: " << i << " j: " << j << " k: " << k << " l: " << l << endl;


							fill(actionVector.begin(), actionVector.end(), 0);
							actionVector[i] = xi;
							actionVector[j] = xj;
							actionVector[k] = xk;
							actionVector[l] = xl;
							par1.clear();
							for(int p = 0; p <nAccts;p++){
								par1.push_back(par[p] + actionVector[p]);

							}

							int negFlag = 0;
							for (int p = 0; p < nAccts; p++){
								if(par1[p] < 0){ negFlag -= 1;}
							}

							if(negFlag == 0){
								for (int p = 0; p < nAccts; p++){
									netPosAcct2_ins[p] = sgnNet * par1[p];
								}
								netPosAcct2_ins = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2_ins, scale, qt);
								vector < double > pnlAcctSoFar2;
								pnlAcctSoFar2.clear();
								for (int p = 0; p < nAccts; p++ ){
									pnlAcctSoFar2.push_back(pnlAcctSoFar[p] + netPosAcct2_ins[p] * (priceNow - pricePre) * onePtValue);
								}

								double mae = criterion(pnlAcctSoFar2, cumPnl, af, 1);
								if (mae == minMae){
									double e1, e2;
									vector <double > mid1, mid2;
									transform( netPosAcct2_ins.begin(), netPosAcct2_ins.end(), af.begin(), back_inserter(mid1), opDiv);
									e1 = entropy( mid1 );
									transform( netSoFar.begin(), netSoFar.end(), af.begin(), back_inserter(mid2), opDiv);
									e2 = entropy( mid2 );
									if(e1 < e2){
										flag = 1;
									}
								}

								if (mae < minMae || flag){
									minMae = mae;
									optimalCase.push_back(i);
									optimalCase.push_back(j);
									optimalCase.push_back(k);
									optimalCase.push_back(actionVector[i]);
									optimalCase.push_back(actionVector[j]);
									optimalCase.push_back(actionVector[k]);
									vector<double> qtAcct;
									qtAcct.clear();
									for(int p = 0; p <netPosAcct2_ins.size(); p++){
										qtAcct.push_back(netPosAcct2_ins[p] - netPosAcct[p]);
									}
									for (int p = 0; p < nAccts; p++){
										optimalAllocation[p] = fabs(qtAcct[p]);
									}
									netSoFar = netPosAcct2_ins;
									flag = 0;

								}
							}
						}
					}
				}
			}
		}

	}

	return;
}