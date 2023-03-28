//
// Created by 王楚乔 on 12/4/17.
//

#include "allInclude.h"

static double entropy(vector<double> b){
    int N = b.size();
    double entropy = 0;
    for(int i = 0;i < N;i++){
        for(int j = i+1; j < N; j++){
            entropy +=  (b[i] - b[j])*(b[i] - b[j]);
        };
    };
    return entropy;

}


