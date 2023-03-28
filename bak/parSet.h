/*
* @Author: shayaan
* @Date:   2020-06-11 15:58:49
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-11 16:03:42
*/

#ifndef CLION_PARSET_H
#define CLION_PARSET_H

#include "allInclude.h"
#include "Entropy.cpp"
#include "sanityCheckOnNetPosAcc.cpp"
#include "criterion.cpp"


static double opDiv(double a, double b){
   return a/b;
}


class parSet {
public:
	vector< double > optimalAllocation;
	vector< double > netPosAcct2;
	int sgnNet;
	vector< double > af;
	double cumPnl;
	vector<double> pnlAcctSoFar;
	vector<double> netPosAcct;
	int scale;
	int qt;
	double pricePre;
	double priceNow;
	int onePtValue;
	char criteriaOption;
	double sanityOnNetPosition;
	double minMae;
	vector<double > netSoFar;
	vector<int> perturbVec;
	vector<double> optimalCase;


	parSet(int n,vector<double> allocation,int onePtValue);
	void optimizingPnlByPerturbingNetPos();

};


#endif //CLION_PARSET_H
