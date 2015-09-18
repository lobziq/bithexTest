#pragma once

#include <iostream>
#include <sstream>
#include "converter.h"

class bitObject
{
public:
	bitObject(int type, std::string data);

	std::string getValue();

	static int isValidPacket(std::string data);

	~bitObject();

	std::string data;

private:
	void generateValue();

	void createType1Value();
	void createType2Value();
	void createType3Value();
	void createType4Value();

	int type;
	//std::string data;
	std::string value;
};

