#include "stdafx.h"
#include "converter.h"


converter::converter()
{
	
}

std::string converter::binaryToHex(std::string binary)
{
	if (binary.length() != 4)
	{
		//std::cout << "incorrect byte value (length != 4)" << std::endl;
		return "INCORRECT DATA LENGTH (NOT EQUAL 4)";
	}

	std::map <std::string, std::string> m;

	m["0000"] = "0";
	m["0001"] = "1";
	m["0010"] = "2";
	m["0011"] = "3";
	m["0100"] = "4";
	m["0101"] = "5";
	m["0110"] = "6";
	m["0111"] = "7";
	m["1000"] = "8";
	m["1001"] = "9";
	m["1010"] = "A";
	m["1011"] = "B";
	m["1100"] = "C";
	m["1101"] = "D";
	m["1110"] = "E";
	m["1111"] = "F";

	if (m[binary] == "")
	{
		//std::cout << "Incorrect byte value" << std::endl;
		return "INCORRECT BINARY VALUE";
	}

	return m[binary];
}

std::string converter::hexToReversedBinary(char hex)
{
	switch (hex)
	{
	case '0':
		return "0000";
	case '1':
		return "1000";
	case '2':
		return "0100";
	case '3':
		return "1100";
	case '4':
		return "0010";
	case '5':
		return "1010";
	case '6':
		return "0110";
	case '7':
		return "1110";
	case '8':
		return "0001";
	case '9':
		return "1001";
	case 'A':
		return "0101";
	case 'B':
		return "1101";
	case 'C':
		return "0011";
	case 'D':
		return "1011";
	case 'E':
		return "0111";
	case 'F':
		return "1111";
	default:
		return "INCORRECT HEX VALUE";
	}
}

std::string converter::hexToBinary(char hex)
{
	switch (hex)
	{
		case '0':
			return "0000";
		case '1':
			return "0001";
		case '2':
			return "0010";
		case '3':
			return "0011";
		case '4':
			return "0100";
		case '5':
			return "0101";
		case '6':
			return "0110";
		case '7':
			return "0111";
		case '8':
			return "1000";
		case '9':
			return "1001";
		case 'A':
			return "1010";
		case 'B':
			return "1011";
		case 'C':
			return "1100";
		case 'D':
			return "1101";
		case 'E':
			return "1110";
		case 'F':
			return "1111";
		default:
			return "INCORRECT HEX VALUE";
	}

	/*old implementation, suddenly impossibru slow
	std::map <char, std::string> m;

	m['0'] = "0000";
	m['1'] = "0001";
	m['2'] = "0010";
	m['3'] = "0011";
	m['4'] = "0100";
	m['5'] = "0101";
	m['6'] = "0110";
	m['7'] = "0111";
	m['8'] = "1000";
	m['9'] = "1001";
	m['A'] = "1010";
	m['B'] = "1011";
	m['C'] = "1100";
	m['D'] = "1101";
	m['E'] = "1110";
	m['F'] = "1111";

	if (m[hex] == "")
	{
		//std::cout << "Incorrect byte value" << std::endl;
		return "INCORRECT HEX VALUE";
	}

	return m[hex]; */
}

std::string converter::decimalToHex(unsigned short dec)
{
	std::string output;
	char c[7] = "ABCDEF";
	unsigned short n;
	unsigned short oldn = dec;
	unsigned short remainder;

	for (int i = 0; i < 4; i++)
	{
		n = oldn / 16;
		remainder = oldn - (16 * n);
		oldn = n;
		if (remainder > 9)
		{
			output = c[remainder - 10] + output;
		}
		else
		{
			output = std::to_string(remainder) + output;
		}
	}

	//unsigned short n = dec / 16;
	//unsigned short remainder = dec - (16 * n);

	//std::cout << n << std::endl;

	return output;
}


converter::~converter()
{
}
