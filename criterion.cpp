/*
* @Author: shayaan
* @Date:   2020-06-12 11:48:37
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-15 15:52:53
*/


#include "allInclude.h"

static double criterion(vector<double> pnlAcctSoFar, double cumPnl, vector<double> af, int option){
    double mae = 0;
    double nAccts = pnlAcctSoFar.size();
    switch(option){
       case 0:
           for(int i = 0;i < nAccts; i++){
               for(int j = 0;j < nAccts; j++){
                   if (i!=j){
                       mae += (pnlAcctSoFar[i]/af[i]-pnlAcctSoFar[j]/af[j])*(pnlAcctSoFar[i]/af[i]-pnlAcctSoFar[j]/af[j]);
                   }
               }
           }
           break;
        case 1:
            for(int i = 0;i < nAccts; i++){
                double incr = pnlAcctSoFar[i]/af[i] - cumPnl;
                mae += (incr*incr);
            }
            break;

       case 2:
           for(int i = 0;i < nAccts; i++){
               double incr = fabs((pnlAcctSoFar[i]-af[i]*cumPnl)/af[i]*cumPnl);
               mae += incr;
           };
           break;
       case 3:
           for(int i = 0;i < nAccts; i++){
               double incr = (pnlAcctSoFar[i]-af[i]*cumPnl);
               mae += incr*incr;
           };
           break;

       case 4:
           for(int i = 0;i < nAccts; i++){
               double incr = fabs(pnlAcctSoFar[i]-af[i]*cumPnl);
               mae += incr;
           };
           break;

    }
    return mae;
}