/*
* @Author: shayaan
* @Date:   2020-06-12 14:58:49
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-15 08:03:54
*/


#include "allInclude.h"
#include "readAttributes.h"
#include "ordersStruct.h"

static vector< vector<double>>  filledOrdersManager (string filename, string symbol2, string whichAction, int whichOrder) {
		int sum_buy = 0,sum_sell=0;

		readAttributes readAttribute(filename);
		int numOfInstruments = readAttribute.getNAttributes();
		cout << "Total number of trades is : " << numOfInstruments << endl;
		string **matrix = readAttribute.getAttributes();
	
		ordersStruct orders(matrix, numOfInstruments);

		vector<double> output1_time, output2, output4_price, output5_buy, output6_sell;
		vector<string> output3_symbol;

		for (int i = 0; i < numOfInstruments; i++) {
			output1_time.push_back(orders.time[i]);//time
			output2.push_back(orders.lastPrice[i]);//lastPrice
			output3_symbol.push_back(orders.symbol[i]);//symbol
			output4_price.push_back(orders.lastPrice[i]);


			if (orders.side[i] == 1) {
				output6_sell.push_back(orders.lastShares[i]);
				output5_buy.push_back(0);
				sum_sell+=orders.lastShares[i];
			} else {
				output5_buy.push_back(orders.lastShares[i]);
				output6_sell.push_back(0);
				sum_buy+=orders.lastShares[i];
			}

		}

	cout<<"Sell:"<<sum_sell<<" Buy:"<<sum_buy<<endl;

	vector< vector<double>>  RESULT;

	if (whichAction == "contract"){
		int len = output1_time.size();
		vector<double> buy,sell,time,price;
		for(int j=0;j<len;j+=1){
			price.push_back(output4_price[j]);
			time.push_back(output1_time[j]);
			// cout<<output4_price[j]<<endl;
			if(output5_buy[j] > 0){
				buy.push_back(output5_buy[j]);
				sell.push_back(0);
			}

			else if(output6_sell[j] > 0){
				buy.push_back(0);
				sell.push_back(output6_sell[j]);
			}
		}
		RESULT.push_back(time);
		RESULT.push_back(buy);
		RESULT.push_back(sell);
		RESULT.push_back(price);
	}

	else if (whichAction == "expand"){
		int count = 0;
		int len = output1_time.size();
		vector <double> buy, sell, price, time, dataOverwrite1, dataOverwrite2, dataOverwrite4, dataOverwrite5, dataOverwrite6;
		vector <string> dataOverwrite3;
		for(int j = 0; j < len; j++){
			for(int k = 0; k < max(output5_buy[j], output6_sell[j]); k++){
				count += 1;
				if (output5_buy[j] > 0){
					buy.push_back(1);
					sell.push_back(0);
				}else if(output6_sell[j] > 0){
					buy.push_back(0);
					sell.push_back(1);
				}
				price.push_back(output4_price[j]);
				time.push_back(output1_time[j]);
				dataOverwrite1.push_back(output1_time[j]);
				dataOverwrite2.push_back(output2[j]);
				dataOverwrite3.push_back(output3_symbol[j]);
				dataOverwrite4.push_back(output4_price[j]);
				if (output5_buy[j] > 0){
					dataOverwrite5.push_back(1);
					dataOverwrite6.push_back(0);
				}else if(output6_sell[j] > 0){
					dataOverwrite5.push_back(0);
					dataOverwrite6.push_back(1);
				}

			}
		}

		RESULT.push_back(time);
		RESULT.push_back(buy);
		RESULT.push_back(sell);
		RESULT.push_back(price);
		RESULT.push_back(dataOverwrite1);
		RESULT.push_back(dataOverwrite2);
		RESULT.push_back(dataOverwrite4);
		RESULT.push_back(dataOverwrite5);
		RESULT.push_back(dataOverwrite6);
	}

	else{
		cout << "Option does not exist" << endl;
	}

	return RESULT;
}
