/*
* @Author: shayaan
* @Date:   2020-06-19 14:49:20
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-19 16:14:08
*/

#include "allInclude.h"

struct symbolStruct{
	string symbol;
	string description;
	int onePtValue;
};


int readOnePtValue(string ticker,string fileName){
	vector<symbolStruct> data;
	int value;
	ifstream inputFile;
	inputFile.open(fileName.c_str());
	string hold;
	getline(inputFile,hold);

	string temp;

	while(getline(inputFile,hold)){
		int count = 0;
		symbolStruct temp_data;
		stringstream ss(hold);
		while(getline(ss,temp,',')){
			if(count == 0){
				temp_data.symbol = temp;
			}
			else if(count == 1){
				temp_data.description = temp;
			}
			else{
				temp_data.onePtValue = stoi(temp);
			}
			count+=1;
		}
		data.push_back(temp_data);
	}
	

	for(int i=0;i<data.size();i+=1){
		if(data[i].symbol == ticker){
			value = data[i].onePtValue;
		}
	}

	return value;
}