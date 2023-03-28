/*
* @Author: shayaan
* @Date:   2020-06-12 16:00:54
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-12 16:01:05
*/


#include "allInclude.h"
#include "criterionNetPosition.cpp"

static vector<double> lastTradeAllocation(int qty, int scale, vector<double> af, vector <double> netPosAcct, int scenario){
    int nAccts = af.size();
    vector <double> allocation;
    switch (scenario){
        case 1:
            for(int i = 0; i < nAccts; i++){
                allocation.push_back(fabs(netPosAcct[i]));
            }
            int sum;
            for(int i = 0; i < nAccts; i++){
                sum += allocation[i];
            }
            if(sum != 0){
                cout << "something is not right" << endl;
            }
            break;
        case 2: {
            vector<double> par0;
            vector<double> par;
            double sum;
            for (int i = 0; i < nAccts; i++) {
                par0.push_back(af[i] * qty);
                par.push_back(round(af[i] * qty));
                sum += round(af[i] * qty);
            }
            while(sum != qty){
                if(sum > qty){
                    vector<int> minIndex = {0};
                    double min = par0[0];
                    for(int i = 0; i < par0.size(); i++){
                        if (par0[i] < min){
                            min = par0[i];
                            minIndex.clear();
                            minIndex.push_back(i);
                        }
                        if (par0[i] == min){
                            minIndex.push_back(i);
                        }
                    }
                    if(par[minIndex[0]] > 0){
                        par[minIndex[0]] -= 1;
                    }

                }else{
                    vector<int> maxIndex = {0};
                    double max = par0[0];
                    for(int i = 0; i < par0.size(); i++){
                        if (par0[i] > max){
                            max = par0[i];
                            maxIndex.clear();
                            maxIndex.push_back(i);
                        }
                        if (par0[i] == max){
                            maxIndex.push_back(i);
                        }
                    }
                    par[maxIndex[0]] += 1;
                }
            }
            allocation = par;
            break;
        }
        case 4: {
            vector<double> par0;
            vector<double> par;
            int sum;
            for (int i = 0; i < nAccts; i++) {
                par0.push_back(af[i] * qty);
                par.push_back(round(af[i] * qty));
                sum += round(af[i] * qty);
            }
            while (sum != qty) {
                if (sum > qty) {
                    vector<int> minIndex = {0};
                    double min = par0[0];
                    for (int i = 0; i < par0.size(); i++) {
                        if (par0[i] < min) {
                            min = par0[i];
                            minIndex.clear();
                            minIndex.push_back(i);
                        }
                        if (par0[i] == min) {
                            minIndex.push_back(i);
                        }
                    }
                    par[minIndex[0]] -= 1;

                } else {
                    vector<int> maxIndex = {0};
                    double max = par0[0];
                    for (int i = 0; i < par0.size(); i++) {
                        if (par0[i] > max) {
                            max = par0[i];
                            maxIndex.clear();
                            maxIndex.push_back(i);
                        }
                        if (par0[i] == max) {
                            maxIndex.push_back(i);
                        }
                    }
                    par[maxIndex[0]] += 1;

                }

            }

            vector<double> netPosAcct2;
            for (int i = 0; i < netPosAcct.size(); i++) {
                netPosAcct2.push_back(netPosAcct[i] + scale * par[i]);
            }
            double mae2 = criterionNetPosition(netPosAcct2, af);
            double minMae = mae2;
            allocation = par;
            for (int i = 0; i < nAccts; i++){
                for(int j = 0; j < nAccts; j++){
                    if (i != j){
                        vector <double> actionVector(nAccts,0);
                        actionVector[i] = 1;
                        actionVector[j] = -1;
                        vector <double> par1;
                        double par1NegFlag = 0;
                        for(int m = 0; m < nAccts; m++){
                            par1[m] = par[m] + actionVector[m];
                            if (par1[m] < 0){
                                par1NegFlag -= 1;
                            }
                        }
                        if(par1NegFlag < 0){
                            par1 = par;
                        }
                        for (int m = 0; m < nAccts; m++){
                            netPosAcct2[m] = netPosAcct[m] + scale*par1[m];
                        }
                        mae2 = criterionNetPosition(netPosAcct2, af);
                        if (mae2 < minMae){
                            minMae = mae2;
                            allocation = par1;
                        }
                    }
                }
            }
            break;
        }
        case 3: {
            double netPosSystem = scale * qty;
            for(int i = 0; i < nAccts; i++){
                netPosSystem += netPosAcct[i];
            }
            int sgnNet = 1;
            if(netPosSystem < 0){
                sgnNet = -1;
            }
            vector<double> par;
            double sum = 0;
            for(int i = 0; i < nAccts; i++){
                par.push_back(round(af[i] * fabs(netPosSystem)));
                sum += par[i];
            }
            while (sum != fabs(netPosSystem)){
                if (sum > fabs(netPosSystem)){
                    vector<int> maxIndex = {0};
                    double max = par[0];
                    for(int k = 0;  k < par.size(); k++){
                        if (par[k] > max){
                            max = par[k];
                            maxIndex.clear();
                            maxIndex.push_back(k);
                        }
                        if (par[k] == max){
                            maxIndex.push_back(k);
                        }
                    }
                    par[maxIndex[0]] -= 1;

                }else{
                    vector<int> maxIndex = {0};
                    double max = par[0];
                    for(int k = 0;  k < par.size(); k++){
                        if (par[k] > max){
                            max = par[k];
                            maxIndex.clear();
                            maxIndex.push_back(k);
                        }
                        if (par[k] == max){
                            maxIndex.push_back(k);
                        }
                    }
                    par[maxIndex[0]] += 1;

                }

            }
            vector<double> netPosAcct2;
            vector<double> qtAcct;
            for(int k = 0; k < nAccts; k++){
                netPosAcct2.push_back(par[k] * sgnNet);
            }
            for(int k = 0; k < nAccts; k++){
                qtAcct.push_back(netPosAcct2[k] - netPosAcct[k]);
            }
            netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, scale, qty);
            for(int k = 0; k < nAccts; k++){
                // cout<<k<<allocation.size()<<endl;
                qtAcct[k] = (netPosAcct2[k] - netPosAcct[k]);
                allocation.push_back(fabs(qtAcct[k]));
            }

            break;
        }

    }
    return allocation;

}