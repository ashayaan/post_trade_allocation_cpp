//
// Created by 王楚乔 on 12/4/17.
//

#include "allInclude.h"

static vector<double> sanityCheckOnNetPosAcct(vector<double> netPosAcct, vector<double> netPosAcct2, int scale, int qt){
    vector<double> diffVec;
    for(unsigned int i = 0; i < netPosAcct.size(); i++){
        diffVec.push_back(netPosAcct2[i] - netPosAcct[i]);
    }
    int sumFlagNeg = 0;
    int sumFlagPos = 0;

    for(unsigned int i = 0; i < diffVec.size(); i++){
        if(diffVec[i] < 0){
            sumFlagNeg += 1;
        };
        if(diffVec[i] > 0){
            sumFlagPos += 1;

        };
    }

    //cout<<"Scale: "<<scale<<" "<<sumFlagPos<<" "<<sumFlagNeg<<endl;
    

    if (scale > 0 && sumFlagNeg > 0){
        for(unsigned int i = 0; i < diffVec.size(); i++){
            if (diffVec[i] < 0){
                netPosAcct2[i] = netPosAcct[i];
            };
        };

        double sumdif = 0;
        vector<double> tmp;

        for(unsigned int i = 0; i<diffVec.size(); i++){
            sumdif = sumdif + netPosAcct2[i] - netPosAcct[i];
        };

        while(sumdif != scale*qt){
            tmp.clear();
            for(unsigned int i = 0; i<diffVec.size(); i++){
                tmp.push_back(netPosAcct2[i] - netPosAcct[i]);
            };

            int indexJ;
            int Max = 0;
            for(unsigned int i = 0; i<tmp.size(); i++){
                if (tmp[i] >= Max){
                    indexJ = i;
                    Max = tmp[i];
                }
            };
            netPosAcct2[indexJ] -= 1;
            sumdif -=1 ;

        }

    }

    else if (scale < 0 && sumFlagPos > 0){
        for(unsigned int i = 0; i < diffVec.size(); i++){
            if (diffVec[i] > 0){
                netPosAcct2[i] = netPosAcct[i];
            };
        };

        double sumdif = 0;
        vector<double> tmp;

        for(unsigned int i = 0; i<diffVec.size(); i++){
            sumdif += netPosAcct2[i] - netPosAcct[i];
        };

        while(sumdif != scale*qt){
            tmp.clear();

            for(unsigned int i = 0; i<diffVec.size(); i++){
                tmp.push_back(netPosAcct2[i] - netPosAcct[i]);
            };
            int indexJ;
            int Min = 0;
            for(unsigned int i = 0; i< tmp.size(); i++){
                if (tmp[i] <= Min){
                    indexJ = i;
                    Min = tmp[i];
                }
            };
            netPosAcct2[indexJ] += 1;
            sumdif += 1;

        }

    }

    return netPosAcct2;


}
