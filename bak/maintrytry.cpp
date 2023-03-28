#include "allInclude.h"
#include "postTradeAllocation.cpp"
#include "accounts.h"
#include "readAccountDetails.cpp"



int main(int argc, char const *argv[]) {
	if(argc != 6){
		cout<<"Usage:"<<endl;
		cout<<"./readData.out, Account details file, Data file, Action, Symbol, Criterion"<<endl;
		return 0;
	}
	vector<string> all_args;
	all_args.assign(argv + 1, argv + argc);
	
	char criteriaOption = all_args[4][0];
	int sanityOnNetPosition = 0;
	string accountFile = all_args[0];

	vector<accountDetails> accounts;
	readAccountDetails(accounts,accountFile);
	if(accounts.size() == 0){
		cout<<"Failed to read the account data"<<endl;
		return 0;
	}

	string fileName = all_args[1];
	string symbol2 = "FGBLZ1";
	vector <double> rollDate;
	rollDate.clear();

	string whichAction = all_args[2];
	int whichOrder = 1;

	// cout<<accountFile<<" "<<fileName<<" "<<criteriaOption<<" "<<whichAction<<endl;
	postTradeAllocation(accounts, fileName, symbol2, rollDate, whichAction, criteriaOption, sanityOnNetPosition, whichOrder);

	return 0;

}
