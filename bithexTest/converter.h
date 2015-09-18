#pragma once

#include <iostream>
#include <map>
#include <string>

class converter
{
public:
	converter();

	static const std::map <std::string, std::string> conversionMap;

	static std::string binaryToHex(std::string byte);
	static std::string decimalToHex(unsigned short dec);
	static std::string hexToBinary(char hex);
	static std::string hexToReversedBinary(char hex);


	~converter();

private:
};

