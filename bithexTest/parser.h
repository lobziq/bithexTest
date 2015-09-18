#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "bitObject.h"
#include "converter.h"


class parser
{
public:
	parser();

	bool prepareString(std::string input, unsigned int maxLength);
	void parseString(std::string input, unsigned int maxLength);
	void invertString(std::string* s);

	void transferIsOver();

	std::string getParsed();

	~parser();

private:
	bool lastChance = false;
	unsigned int limit;

	std::string leftOver;
	std::string inputString;
	std::string binaryString;
	std::vector <bitObject> objects;
};

