
#include "allInclude.h"
#include "readAttributes.h"


//Constructor
readAttributes::readAttributes(string inputFileName){
	
	countNAttributesFieldsInputFile(inputFileName);

	fields = new string[nFields];
	attributes = new string*[nAttributes];
	for (int iAttributes = 0; iAttributes < nAttributes; iAttributes++){
		attributes[iAttributes] = new string[nFields];
	}

	extractInfo(inputFileName, attributes, fields);
	string marketField = "SIDE";
	findIndexMarketField(marketField);
}

//Destructor
readAttributes::~readAttributes(){
	for (int iAttributes = 0; iAttributes < nAttributes; iAttributes++){
		delete[] attributes[iAttributes];
	}
	delete[] attributes;
	delete[] fields;
}

// member function getNFields
int readAttributes::getNFields(){
	return nFields;
}

// member function getNAttributes
int readAttributes::getNAttributes(){
	return nAttributes;
}

// member function getFields
string* readAttributes::getFields(){
	return fields;
}

// member function getAttributes	
string** readAttributes::getAttributes(){
	return attributes;
}

// member function countNAttributesFieldsInputFile
void readAttributes::countNAttributesFieldsInputFile(string inputFileName){
	
	ifstream inputFile;
	inputFile.open(inputFileName.c_str());
	if (!inputFile.is_open()){
		cout << "could not open the input file.";
		return;
	}
	string headerLine;
	getline(inputFile, headerLine);
	stringstream lineToProcess(headerLine);
	string attribute;
	nFields = 0;
	while (getline(lineToProcess, attribute, ',')){
		nFields++;
	}

	string fieldLine;
	nAttributes = 0;
	while(getline(inputFile, fieldLine)){
		nAttributes++;
	}

	inputFile.close();
}

// member function extractInfo
void readAttributes::extractInfo(string inputFileName, string **attributes, string *fields){

	if (nAttributes == 0){
		cout << "The number of attributes in the input file is 0." << endl;
		return;
	}

	ifstream inputFile;
	inputFile.open(inputFileName.c_str());
	if (!inputFile.is_open()){
		cout << "could not open the input file.";
		return;
	}

	string line;
	getline(inputFile, line);
	stringstream lineToProcess(line);
	string field;
	for (int iFields = 0; iFields < nFields; iFields++){
		getline(lineToProcess, field, ',');
		fields[iFields] = field;
	}
	
	string attribute;
	for (int iAttributes = 0; iAttributes < nAttributes; iAttributes++){
		getline(inputFile, line);
		stringstream lineToProcess(line);
		for (int iFields = 0; iFields < nFields; iFields++){
			getline(lineToProcess, attribute, ',');
			attributes[iAttributes][iFields] = attribute;
		}
	}
	inputFile.close();
}

// member function findIndexMarketField
void readAttributes::findIndexMarketField(string marketField){
	for (int iFields = 0; iFields < nFields; iFields++){
		if (strcmp(fields[iFields].c_str(), marketField.c_str()) == 0){
			indexMarketField = iFields;
			return;
		}
	}
	indexMarketField = -1;
	cout << "Market field was not found in the input file" <<  endl;
}

// member function getMarketAllFields
int readAttributes::getMarketAllFields(string market, string* marketFields){
	for(int iAttributes = 0; iAttributes < nAttributes; iAttributes++){
		if (strcmp(attributes[iAttributes][indexMarketField].c_str(), market.c_str()) == 0){
			for (int iFields = 0; iFields < nFields; iFields++){
				marketFields[iFields] = attributes[iAttributes][iFields];
			}
			return 1;
		}
	}
	cout << "Market not found" << endl;
	for (int iFields = 0; iFields < nFields; iFields++){
		marketFields[iFields] = "N/A";
	}
	return 0;
}

// member function getMarketField
int readAttributes::getMarketField(string market, string field, string &fieldMarket){
	for(int iAttributes = 0; iAttributes < nAttributes; iAttributes++){
		if (strcmp(attributes[iAttributes][indexMarketField].c_str(), market.c_str()) == 0){			
			for (int iFields = 0; iFields < nFields; iFields++){
				if (strcmp(fields[iFields].c_str(), field.c_str()) == 0){
					fieldMarket = attributes[iAttributes][iFields];
					return 1;
				}
			}
			cout << "In " << market << " " << field << " was not found." << endl;
			fieldMarket = "N/A";
			return 0;
		}
	}
	cout << market << " was not found." << endl;
	fieldMarket = "N/A";
	return 0;
}

