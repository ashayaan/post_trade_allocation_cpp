/*
* @Author: shayaan
* @Date:   2020-06-08 10:15:47
* @Last Modified by:   shayaan
* @Last Modified time: 2020-06-08 15:42:16
*/

/* 
This files contains code to read the attributes of the accounts
store it in a vector and return to the main function
*/ 

#include<bits/stdc++.h>
#include "accounts.h"

using namespace std;

int countAttributes(string fileName){
	ifstream inputFile;
	inputFile.open(fileName.c_str());
	
	if(!inputFile.is_open()){
		cout<<"Cannot open file"<<endl;
		return 0;
	}

	string field;
	int nattributes;

	getline(inputFile,field); //reading the header

	while(getline(inputFile,field)){
		nattributes+=1;
	}

	inputFile.close();
	
}

void extractAccountInfo(vector<accountDetails> &accounts, int number_attributes, string fileName){
	ifstream inputFile;
	inputFile.open(fileName.c_str());

	if(!inputFile.is_open()){
		cout<<"Failed to open file"<<endl;
		return;
	}

	string line;
	getline(inputFile,line);

	while(getline(inputFile,line)){
		int tracker =  0;
		stringstream ss(line);
		string substr;
		accountDetails temp;
		while(getline(ss,substr,',')){
			//skipping the ID field of the csv file
			if(tracker == 0){
				tracker+=1;
				continue;
			}

			else if(tracker == 1){
				temp.company = substr;
			}

			else if(tracker == 2){
				temp.account_number = stoi(substr);
			}


			else if(tracker == 3){
				temp.broker = substr;
			}

			else if(tracker == 4){
				temp.percentage = stod(substr);
			}

			else
				break;

			tracker+=1;
		}
		accounts.push_back(temp);
	}
}

void readAccountDetails(vector<accountDetails> &accounts, string fileName){
	cout<< fileName << endl;
	
	int number_attributes = countAttributes(fileName);
	if(number_attributes == 0){
		cout<<"The file is empyt"<<endl;
		return;
	}

	extractAccountInfo(accounts,number_attributes,fileName);
}