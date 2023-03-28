/*
* @Author: shayaan
* @Date:   2020-06-11 23:15:02
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-16 00:28:21
*/

#include "allInclude.h"

static parSet gridSearch2(double cumPnl, double netPosSystem, vector <double> netPosAcct, vector <double> af, int qt,
                   vector <double> pnlAcctSoFar, double pricePre, double priceNow, int onePtValue, int scale,
                   char criteriaOption, double sanityOnNetPosition){

    parSet parSet(af.size(),af,onePtValue);

    int sgnNet;
    if ( netPosSystem > 0){
        sgnNet = 1; 
    }
    if ( netPosSystem < 0){
        sgnNet = -1; 
    }
    
    vector <double> par;
    par.clear();
    
    for (int i = 0; i < af.size(); i++){
        par.push_back(round (af[i] * fabs( netPosSystem )));
    }

    while ( accumulate(begin(par), end(par), 0) != fabs(netPosSystem)){

        if (accumulate(begin(par), end(par), 0) > fabs(netPosSystem)){
            int max = par[0]; //setting seed for maximum
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
        parSet.optimalAllocation[i] = fabs(qtAcct[i]);
    }

    netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, scale, qt);
    
    qtAcct.clear();

    for(int i = 0; i < netPosAcct2.size(); i++){
        qtAcct.push_back(netPosAcct2[i] - netPosAcct[i]);
    }

    for(int i = 0; i < qtAcct.size(); i++){
        parSet.optimalAllocation[i] = fabs(qtAcct[i]);
    }

    double sumqtAcct = 0;

    for(int i = 0; i < qtAcct.size(); i++){
        sumqtAcct += qtAcct[i];
    }

    if((scale < 0 && sumqtAcct > 0)>0 || (scale > 0 && sumqtAcct < 0) > 0 || sumqtAcct != scale * qt){
        cout << "ERROR: In gridSreach2 - something is not right ..." << endl;
    }


    parSet.netPosAcct2 = netPosAcct2;
    parSet.sgnNet = sgnNet;
    parSet.af = af;
    parSet.cumPnl = cumPnl;
    parSet.pnlAcctSoFar = pnlAcctSoFar;
    parSet.netPosAcct = netPosAcct;
    parSet.scale = scale;
    parSet.qt = qt;
    parSet.pricePre = pricePre;
    parSet.priceNow = priceNow;
    parSet.onePtValue = onePtValue;
    parSet.criteriaOption = criteriaOption;
    parSet.sanityOnNetPosition = sanityOnNetPosition;

    vector <double> pnlAcctSoFar2;
    pnlAcctSoFar2.clear();

    for (int i = 0; i < pnlAcctSoFar.size(); i++ ){
        pnlAcctSoFar2.push_back( pnlAcctSoFar[i] + netPosAcct2[i] * (priceNow - pricePre) * onePtValue); // PnL with new positions

    }



    parSet.minMae = criterion(pnlAcctSoFar2, cumPnl, af, criteriaOption-'0');




    parSet.optimalCase.clear();
    parSet.netSoFar = netPosAcct2;


    parSet.perturbVec = {1, -1};
    parSet.optimizingPnlByPerturbingNetPos();
    // cout<<"One Done"<<endl;

    parSet.perturbVec = {1, -1, 1, -1};
    parSet.optimizingPnlByPerturbingNetPos();
    // cout<<"Two Done"<<endl;
    parSet.perturbVec = {-1, -1, 2};
    parSet.optimizingPnlByPerturbingNetPos();

    parSet.perturbVec = {1, 1, -2};
    parSet.optimizingPnlByPerturbingNetPos();

    parSet.perturbVec = {2,-2};
    parSet.optimizingPnlByPerturbingNetPos();

    parSet.perturbVec = {3, -3};
    parSet.optimizingPnlByPerturbingNetPos();

    parSet.perturbVec = {1, 2, -3};
    parSet.optimizingPnlByPerturbingNetPos();

    parSet.perturbVec = {3, -1, -2};
    parSet.optimizingPnlByPerturbingNetPos();

    return parSet;

}
