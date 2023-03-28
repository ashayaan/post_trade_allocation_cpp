#include "allInclude.h"
#include "postTradeAllocation.cpp"
#include "accounts.h"
#include "readAccountDetails.cpp"

void createFile(string name,vector<accountDetails> accounts){
	fstream file; 
	file.open(name,ios::out); 
	
	for(int i=0;i<accounts.size();i+=1){
		if(i == accounts.size()-1)
			file<<accounts[i].account_number;
		else
			file<<accounts[i].account_number<<",";
	}
	file<<"\n";

	if(!file){ 
		cout<<"Error in creating file!!!"; 
		return; 
	}
}


int main(int argc, char const *argv[]) {
	if(argc != 8){
		cout<<"Usage:"<<endl;
		cout<<"./readData.out, Account details file, Data file, Symbol file, Action, Symbol, Criterion, Output Location"<<endl;
		return 0;
	}
	vector<string> all_args;
	all_args.assign(argv + 1, argv + argc);
	
	char criteriaOption = all_args[5][0];
	int sanityOnNetPosition = 0;
	string accountFile = all_args[0];
	cout<<accountFile<<endl;
	vector<accountDetails> accounts;
	readAccountDetails(accounts,accountFile);
	if(accounts.size() == 0){
		cout<<"Failed to read the account data"<<endl;
		return 0;
	}

	string fileName = all_args[1];
	string symbolFile = all_args[2];
	string ticker = all_args[4];
	vector <double> rollDate;
	rollDate.clear();

	string whichAction = all_args[3];
	int whichOrder = 1;
	
	time_t now = time(0);
	tm *ltm = localtime(&now);
	
	//Creating output file
	string output_file_positions = all_args[6] + "/positions" + to_string(1900+ltm->tm_year)+to_string(1 + ltm->tm_mon)+to_string( ltm->tm_mday)+".csv";
	string output_file_returns =all_args[6] + "/returns" + to_string(1900+ltm->tm_year)+to_string(1 + ltm->tm_mon)+to_string( ltm->tm_mday)+".csv";
 	createFile(output_file_positions,accounts);
	createFile(output_file_returns,accounts);

	// cout<<accountFile<<" "<<fileName<<" "<<criteriaOption<<" "<<whichAction<<" "<<symbolFile<<" "<<ticker<<endl;
	postTradeAllocation(accounts, fileName, symbolFile, rollDate, whichAction, criteriaOption, sanityOnNetPosition, whichOrder, ticker, output_file_positions, output_file_returns);

	return 0;

}
