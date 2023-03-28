//
// Created by 王楚乔 on 12/8/17.
//

#include "allInclude.h"
#include "readAttributes.h"
#include "ordersStruct.h"

//不用了！！！！！


vector < vector<double> >  readFills (string filename){

    readAttributes readAttribute(filename);
    int numOfInstruments = readAttribute.getNAttributes();
    cout << "Number of trades is : " << numOfInstruments << endl;
    string **matrix = readAttribute.getAttributes();

    ordersStruct orders(matrix,numOfInstruments);
    vector<double> output1_time, output2, output4_price, output5_buy, output6_sell;
    vector<string> output3_symbol; //WE ACTUALLY FORGET ABOUT IT!!!!

    for(int i = 0; i < numOfInstruments; i++){
        output1_time.push_back(orders.time[i]);//time
        output2.push_back(orders.lastPrice[i]);//lastPrice
        output3_symbol.push_back(orders.symbol[i]);//symbol
        output4_price.push_back(orders.lastPrice[i]);
        if(orders.side[i] == 0){
            output6_sell.push_back(orders.lastShares[i]);
            output5_buy.push_back(0);
        }else{
            output5_buy.push_back(orders.lastShares[i]);
            output6_sell.push_back(0);
        }

    }

    return ;
}