/*
* @Author: shayaan
* @Date:   2020-06-11 16:05:14
* @Last Modified by:   shayaan
* @Last Modified time: 2020-08-24 22:15:31
*/

#include "allInclude.h"
#include "filledOrdersManager.cpp"
#include "parSet.h"
#include "accounts.h"
#include "gridSearch2.cpp"
#include "lastTradeAllocation.cpp"
#include "readOnePointValue.cpp"




static void postTradeAllocation(vector<accountDetails> accounts, string fileName, string symbolFile, vector <double> rollDate,
		string whichAction,char criteriaOption,double sanityOnNetPosition, int whichOrder, string ticker,string output_file_positions, string output_file_returns){

	cout<<"Ticker is: "<<ticker<<endl;
	string symbol;
	int nAccts = accounts.size();
	

	vector<double> af;
	for(int i=0;i<accounts.size();i+=1){
		af.push_back(accounts[i].percentage);
	}

	int onePtValue = 1000; //Default
	onePtValue = readOnePtValue(ticker,symbolFile);
	// onePtValue = 1000; //Default
	cout<<"One Point Value "<<onePtValue<<endl;

	// cout<<"onePtValue: "<<onePtValue<<endl;
	//TODO make changes to this
	
	vector< vector <double> > INPUT = filledOrdersManager(fileName, symbolFile, whichAction, whichOrder);
	vector<double> time = INPUT[0];
	vector<double> buy = INPUT[1];
	vector<double> sell = INPUT[2];
	vector<double> price =  INPUT[3];


	int start = 0;
	int len1 = time.size();
	int finish = start + len1;
	int len2 = finish - start + 1;
	//EXCEL TIME TO UNIX TIME;
	long a = ((long) time[start]-70*365-19)*86400-8*3600;
	long *b = &a;
	long c = ((long) time[finish-1]-70*365-19)*86400-8*3600;
	long *d = &c;
	cout << "From: " << ctime(b) << "To:   " << ctime(d) << endl;
	cout << "Length " << len1<<endl;
	vector <double> netPosition;
	vector <double> pnl ;
	vector <double> cumPnl;
	vector< vector<double> > netPosAcct(nAccts,vector<double>(len1,0));
	vector< vector<double> > cumPnlAcct(nAccts,vector<double>(len1,0));

	vector <double> lastPeriodAcctPnL;




	int endDayOn = 0;

	vector <string> allocator; 
	int counter = 0;
	double lastPeriodPnL;
	vector<double> pnlAcctSoFar;
	int scale;
	int qty;
	vector<double> netPosAcct_(nAccts,0);
	vector <double> optimalAllocation;
	parSet parSet(nAccts,af,onePtValue);

	double temp_sum = 0;
	for(int j = 0; j < len1; j++){

		if(j > 0){

			lastPeriodPnL = netPosition[j-1]*(price[j]-price[j-1])*onePtValue;
			temp_sum += lastPeriodPnL;
			pnl.push_back(lastPeriodPnL);
			cumPnl.push_back(cumPnl[j-1] + lastPeriodPnL);
			netPosition.push_back(netPosition[j-1] + buy[j] - sell[j]);
			// cout<<"J "<<j<<" Net Position:"<<netPosition[j]<<" cumPnl:"<<cumPnl[j]<<" Price:"<<(price[j]-price[j-1])*onePtValue<<endl;
		}

		else{
			lastPeriodPnL = 0;
			pnl.push_back(lastPeriodPnL);
			cumPnl.push_back(0);
			netPosition.push_back(buy[j] - sell[j]);
		}
	   
		if(j > 1){
			pnlAcctSoFar.clear();
			for(int k=0;k<nAccts;k+=1)
				pnlAcctSoFar.push_back(cumPnlAcct[k][j-1]);
		}

		else{
			for(int k=0;k<nAccts;k+=1)
				pnlAcctSoFar.push_back(0);
		}

		if(j>0){

			if(buy[j-1] == 0){
				scale = -1;
				qty = sell[j-1];
			}

			else if(sell[j-1] == 0){
				scale = 1;
				qty = buy[j-1];
			}

			if(j > 1){
				netPosAcct_.clear();
				for(int k=0;k<nAccts;k+=1)
					netPosAcct_.push_back(netPosAcct[k][j-2]);
			}
			
			parSet = gridSearch2(cumPnl[j], netPosition[j-1], netPosAcct_, af, qty, pnlAcctSoFar, price[j-1], price[j], onePtValue, scale, criteriaOption, sanityOnNetPosition);
			optimalAllocation = parSet.optimalAllocation;
			


			if(j > 1){
				for(int k = 0; k < nAccts; k++){
					netPosAcct[k][j-1] = netPosAcct[k][j-2] + scale * optimalAllocation[k];
					// cout<<optimalAllocation[k] * scale<<" ";
				}
			}

			else{
				for(int k = 0; k < nAccts; k++){
					netPosAcct[k][j-1] = scale * optimalAllocation[k]; //All trades have to be in the same direction
					// cout<<optimalAllocation[k] * scale <<" ";
				}

			}

			lastPeriodAcctPnL.clear();
			for(int k = 0; k < nAccts; k++){
				lastPeriodAcctPnL.push_back( netPosAcct[k][j-1]*(price[j]-price[j-1])*onePtValue );
				cumPnlAcct[k][j] = cumPnlAcct[k][j-1] + lastPeriodAcctPnL[k];
			}
		}

		
		if(j>0 and j < len1){
			fstream file;
			file.open(output_file_positions,ios_base::app);
			for(int k=0;k<nAccts;k+=1){
				if(k == nAccts-1)
					file<< netPosAcct[k][j-1];
				else
					file<< netPosAcct[k][j-1]<<",";
				}
			file<<"\n";
			file.close();

			fstream file_returns;
			file_returns.open(output_file_returns,ios_base::app);	
			for(int k=0;k<nAccts;k+=1){
				if(k == nAccts-1)
					file_returns<< cumPnlAcct[k][j-1];
				else
					file_returns<< cumPnlAcct[k][j-1]<<",";
				}
			file_returns<<"\n";
			file_returns.close();
		}
		

		if(j == len1 - 1){

			int scenario;

			if (netPosition[j] == 0){
				scenario = 1;
			}

			else if (netPosition[j-1] == 0){
				scenario = 2;
			}

			else{
				scenario = 3;
			}

			if(buy[j] == 0){
				scale = -1;
				qty = sell[j];
			}else if(sell[j] == 0){
				scale = 1;
				qty = buy[j];
			}

			vector <double> netPosAcctFinal;
			netPosAcctFinal.clear();
			for(int m = 0; m < nAccts; m++){
				netPosAcctFinal.push_back(netPosAcct[m][j - 1]);
			}

			// cout<<"scenario "<<scenario<<endl;

			vector <double> allocation2 = lastTradeAllocation(qty, scale, af, netPosAcctFinal, scenario);
			

			if (rollDate.size() != 0){

			}

			else{
				symbol = fileName;
			}

			cout << "netPosAcct(j,:): ";
			for(int k = 0; k < nAccts; k++){
				netPosAcct[k][j] = netPosAcct[k][j-1] + scale * allocation2[k];
				cout<< netPosAcct[k][j] << " ";
			}
			cout<<endl;

			long e = ((long) time[j]-70*365-19)*86400-8*3600;
			long *f = &e;
			cout << "Time: " << ctime(f) << "minMae: " << parSet.minMae << endl;
			cout << "cumPnl: " << cumPnl[j] << endl;
			cout << "cumPnlAcct: "; 
			
			for(int k=0;k<nAccts;k+=1)
				cout<<cumPnlAcct[k][j]<< " ";
			cout<<endl;

			cout << "PriceLast: " << price[j-1] << endl;
			cout << "PriceNow: " << price[j] << endl;
			cout << "netPosition: " << netPosition[j] << endl;
			 
		}
	
	}

	return;

}
