
#include "ordersStruct.h"
#include "allInclude.h"

ordersStruct::ordersStruct(string **matrix,int n){
    date = new string [n];
    time = new double [n];
    symbol = new string [n];
    lastPrice = new double [n];
    lastShares = new double [n];
    side = new double [n];

    for (int i = 0; i<n; i++){
        date[i] = matrix[i][0];
        time[i] = stod(matrix[i][1]);
        symbol[i] = matrix[i][2];
        lastPrice[i] = stod(matrix[i][3]);
        lastShares[i] = stod(matrix[i][4]);
        side[i] = stod(matrix[i][5]);
    }

}

//Destructor
ordersStruct::~ordersStruct(){
    delete[] date;
    delete[] time;
    delete[] symbol;
    delete[] lastPrice;
    delete[] lastShares;
    delete[] side;
}
