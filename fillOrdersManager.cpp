/*
* @Author: shayaan
* @Date:   2020-06-09 00:41:03
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-10 00:25:52
*/

#include <bits/stdc++.h>
#include "orderStruct.h"

using namespace std;

static int getNAttributes(string fileName){
	int nAttributes = 0;
	ifstream inputFile;
	inputFile.open(fileName.c_str());

	if(!inputFile.is_open()){
		cout << "Failed to open file" <<endl;
		return 0;
	}

	string line;
	getline(inputFile,line);
	while(getline(inputFile,line)){
		nAttributes+=1;
	}
	inputFile.close();
	return nAttributes;

}

/*
data hold is a temporary place to store the data which will be 
expanded based on the criteria and stores in the orders vector
*/
static void extractInfomation(vector<orderStruct> &data_hold, string fileName){
	ifstream inputFile;
	inputFile.open(fileName.c_str());

	if(!inputFile.is_open()){
		cout<<"Failed to open file"<<endl;
		return;
	}

	string line;
	getline(inputFile,line);

	while(getline(inputFile,line)){
		stringstream ss(line);
		orderStruct temp;
		string substr;
		int tracker = 0;
		while(getline(ss,substr,',')){
			if(tracker == 0)
				temp.date = substr;
			else if (tracker == 1)
				temp.time = stod(substr); 
			else if (tracker == 2)
				temp.symbol = substr;
			else if (tracker == 3){
				temp.price = stod(substr);
			}
			else if (tracker == 4)
				temp.qty = stoi(substr);
			else
				temp.side = stoi(substr);

			tracker+=1;
		}
		data_hold.push_back(temp);
	}
	
}

static void fillOrdersManager(vector<orderStruct> &orders, string fileName, string futuresSymbol, string whichAction, int whichOrder){
	
	int nAttributes = getNAttributes(fileName);
	if(nAttributes == 0){
		cout<<"The number of attributes present in the file are 0 "<<endl;
		return;
	}

	vector<orderStruct> data_hold;
	data_hold.clear();
	extractInfomation(data_hold,fileName);

	if(whichAction == "contract"){
		//TODO implement contract action
	}

	else if(whichAction == "expand"){
		orderStruct temp;
		for(int i=0; i<data_hold.size(); i+=1){
			for(int j=0;j<data_hold[i].qty;j+=1){
				temp.date = data_hold[i].date;
				temp.time = data_hold[i].time;
				temp.symbol = data_hold[i].symbol;
				temp.price = data_hold[i].price;
				temp.qty = 1;
				//One in the original data file represents sell order
				if(data_hold[i].side == 1)
					temp.side = -1;
				else
					temp.side = 1;
				orders.push_back(temp);
			}
		}
	}

	// for(int i=0;i<orders.size();i+=1){
	// 	cout<<orders[i].date<<" "<<orders[i].time<<" "<<orders[i].symbol<<" "<<orders[i].price<<" "<<orders[i].qty<<" "<<orders[i].side<<endl;
	// }
}