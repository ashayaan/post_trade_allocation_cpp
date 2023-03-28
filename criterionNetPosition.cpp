/*
* @Author: shayaan
* @Date:   2020-06-12 12:14:49
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-12 12:15:19
*/

#include "allInclude.h"

static double criterionNetPosition(vector<double> netP,vector<double> af){
	int nAccts = netP.size();
	double mae = 0;
	for(int i = 0; i < nAccts; i++){
		for(int j = 0; j < nAccts; j++){
			if(i != j){
				mae += (netP[i]/af[i]-netP[j]/af[j])*(netP[i]/af[i]-netP[j]/af[j]);
			}

		}
	}

	return mae;
}
