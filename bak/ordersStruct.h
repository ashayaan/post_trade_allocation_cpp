//
// Created by 王楚乔 on 12/8/17.
//

#ifndef CLION_ORDERSSTRUCT_H
#define CLION_ORDERSSTRUCT_H


#include "allInclude.h"

class ordersStruct {
public:
    string *date;
    double *time;
    string *symbol;
    double *lastPrice;
    double *lastShares;
    double *side;


    ordersStruct(string **matrix,int n);
    ~ordersStruct();
    ordersStruct(const ordersStruct &);

};


#endif //CLION_ORDERSSTRUCT_H
