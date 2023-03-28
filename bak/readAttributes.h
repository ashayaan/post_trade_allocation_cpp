#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

#ifndef _class_READATTRIBUTES_H
#define _class_READATTRIBUTES_H

class readAttributes{

private:

	int nAttributes;
	int nFields;
	int indexMarketField;

	string *fields;
	string **attributes;

	void countNAttributesFieldsInputFile(string inputFileName);
	void extractInfo(string inputFileName, string **attributes, string *fields);
	void findIndexMarketField(string marketField);

public:

	readAttributes(string inputFileName);
	~readAttributes();

	int getNFields();
	int getNAttributes();
	string* getFields();
	string** getAttributes();
	int getMarketAllFields(string market, string* marketFields);
	int getMarketField(string market, string field, string& fieldMarket);

};

#endif
