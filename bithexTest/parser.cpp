#include "stdafx.h"
#include "parser.h"

#include <algorithm>

parser::parser()
{
	//
	limit = 65535;
	lastChance = false;
}

void parser::invertString(std::string* s)
{
	std::string temp;
	for (int i = s->length() - 1; i >= 0; i--)
	{
		temp += s->at(i);
	}
	*s = temp;
}

void parser::transferIsOver()
{
	lastChance = true;
}

bool parser::prepareString(std::string input, unsigned int maxLength)
{
	this->inputString = input;
	binaryString.clear();

	//checking for all string conditions, converting hex to binary + swapping byte contents
	if (inputString.length() > maxLength)
	{
		std::cerr << "Incorrect input string size (more than " << maxLength << " bytes)" << std::endl;
		return false;
	}
	if (inputString.length() == 0)
	{
		std::cerr << "Input string is empty" << std::endl;
		return false;
	} 
	if ((float)inputString.length() / 2 != (int)inputString.length() / 2)
	{
		std::cerr << "Input string contains number of bits not divisible by 8" << std::endl;
		return false;
	}

	for (int i = 0; i < inputString.length(); i+=2)
	{
		std::string s;
		//std::string binary1 = converter::hexToBinary(inputString.at(i));
		//std::string binary2 = converter::hexToBinary(inputString.at(i + 1));

		//for optimization purposed, we initially converting for reverse bit array
		std::string binary1 = converter::hexToReversedBinary(inputString.at(i));
		std::string binary2 = converter::hexToReversedBinary(inputString.at(i + 1));
		if (binary1 != "INCORRECT HEX VALUE" && binary2 != "INCORRECT HEX VALUE")
		{
			s += binary2;
			s += binary1;

			//more slow, but logical approach
			//s += binary1;
			//s += binary2;
			//std::reverse(s.begin(), s.end());
			binaryString += s;
		}
		else
		{
			std::cerr << "Incorrect symbols in string" << std::endl;
			return false;
		}
	}

	//std::cout << binaryString << std::endl;
	return true;
}

void parser::parseString(std::string input, unsigned int maxLength)
{
	objects.clear();
	
	//debug std::cout << "Input string: " << input << std::endl;
	
	if (!lastChance)
	{
		if (!prepareString(input, maxLength)) return;
	}
	
	binaryString = leftOver + binaryString;
	//debug std::cout << "Input binary string: " << binaryString << std::endl;

	leftOver.clear();

	//parsing actual binary string that we prepared earlier
	std::string checker;
	std::string buffer;
	std::string garbage;

	while (binaryString.length() > 7)
	{
		checker = binaryString.substr(0, 8);
		
		if (checker == "11111111")
		{
			//std::cout << "parsing 1111 etc\n";

			if (garbage.length() > 0)
			{
				objects.push_back(bitObject(4, garbage));
				garbage.clear();
			}

			if (checker == binaryString)
			{
				objects.push_back(bitObject(1, checker));
				binaryString.clear();
			}

			buffer = checker;
			int oneCounter = 8;
			for (int i = 8; i < binaryString.length(); i++)
			{
				if (i == (limit - 1)) //i is index so -1
				{
					//max length - converting into object
					buffer += binaryString[i];
					objects.push_back(bitObject(1, buffer));
					binaryString = binaryString.substr(buffer.length(), binaryString.length() - buffer.length());
					break;
				}
				if (i == binaryString.length() - 1)
				{
					//End of string, if there is no more incoming data from user, creating object, else saving this for next input
					buffer += binaryString[i];


					objects.push_back(bitObject(1, buffer));
					//for future lastChance impelemtation, if needed // leftOver = buffer;	

					binaryString.clear();
					break;
				}
				if (binaryString[i] != '1')
				{
					//end of streak, creating object
					objects.push_back(bitObject(1, buffer));
					binaryString = binaryString.substr(buffer.length(), binaryString.length() - buffer.length());
					break;
				}
				oneCounter++;
				buffer += binaryString[i];
			}
		}
		else if (checker == "01111110")
		{
			int counter = 1;
			long anchor = 8;
			if (garbage.length() > 0)
			{
				objects.push_back(bitObject(4, garbage));
				garbage.clear();
			}

			//trying to find next occurence of 0111111110 byte (element) !!
			//our anchor is at the end of first element
			std::size_t index = binaryString.find("01111110", anchor);

			while (index == anchor)
			{
				//streak of elements
				//adding counter keeping this in memory
				counter++;
				anchor += 8;

				index = binaryString.find("01111110", anchor);	

				if (counter >= limit) break;
			}

			if (counter >= limit)
			{
				//maximum length, creating object
				objects.push_back(bitObject(2, binaryString.substr(0, anchor)));
				binaryString = binaryString.substr(anchor, binaryString.length() - anchor);
				break;
			}

			if (anchor == binaryString.length())
			{
				std::string s = binaryString.substr(0, anchor);
				binaryString = binaryString.substr(anchor, binaryString.length() - anchor);
				objects.push_back(bitObject(2, s));
				break;
			}		

			//streak is over, but we got last index of next element if its exists
			//if element isnt exists, we are just taking all our streak of elements into 2nd type object,
			//and all data past last element we dont need to parse now
			
			//taking all the data between last 2 elements (suspecting its our packet)
			std::string packet = binaryString.substr(anchor, index - anchor);

			if (index != std::string::npos)
			{
				if (bitObject::isValidPacket(packet) == 0)
				{	
					//we checked our data, its legit packet
					//our 2 last elements is the boundaries of packet, but the previously elements (if exists) is 2nd type object
					//so we just substracting this from string and converting
					//also cutting string to repeat from beginning
					if (counter > 1)
					{
						objects.push_back(bitObject(2, binaryString.substr(0, anchor - 8)));
					}
					objects.push_back(bitObject(3, binaryString.substr(anchor - 8, index - (anchor - 16))));
					binaryString = binaryString.substr(index + 8, binaryString.length() - index);

					//std::cout << binaryString << std::endl; // heheh just cheking
				}
				else
				{
					//so it isnt legit packet, then our elements before it going into 2nd object
					//also cutting string to repeat

					objects.push_back(bitObject(2, binaryString.substr(0, anchor)));
					binaryString = binaryString.substr(anchor, binaryString.length() - (anchor));
				}
			}
			else
			{
				//we didnt find our next element
				if (lastChance)
				{
					std::string s = binaryString.substr(0, anchor);
					binaryString = binaryString.substr(anchor, binaryString.length() - anchor);
					objects.push_back(bitObject(2, s));	
				}
				else
				{
					std::string packet = binaryString.substr(anchor, binaryString.length() - anchor);
					if (bitObject::isValidPacket(packet) != 1 && bitObject::isValidPacket(packet) != 3)
					{
						//this can be legit packet later if we find boundary condition (0111111110) later
						leftOver = binaryString;
						binaryString.clear();
					}
					else
					{
						//else just converting our streak of elements into 2nd object and cutting string from then
						std::string s = binaryString.substr(0, anchor);
						binaryString = binaryString.substr(anchor, binaryString.length() - anchor);
						objects.push_back(bitObject(2, s));
					}
				}
			}
		}
		else if (binaryString.length() == 8)
		{
			//smells like garbage...
			//are we suppose to convert it right now? MBY possible to left this and parse with next packet
			garbage += binaryString;

			objects.push_back(bitObject(4, garbage));
			//if we are gonna left this// leftOver = garbage;

			binaryString.clear();
			garbage.clear();
		}
		else
		{
			//more than 8 bits isnt converting, but dont seems we could find some actual data
			//taking our next symbol into garbage to convert it later and keep going from now
			garbage += binaryString[0];
			binaryString.erase(0, 1);
		}

	}

	//leftOver.clear();
	//leftOver = binaryString;
	//std::cout << "Leftover: " << leftOver << std::endl;
}

std::string parser::getParsed()
{
	std::string r;
	
	for (int i = 0; i < objects.size(); i++)
	{
		//more info :)
		//r += "\nBinary data: " + objects[i].data;
		//r += "\nHex data: " + objects[i].getValue();
		//r += " ";

		r += objects[i].getValue();
	}

	return r;
}

parser::~parser()
{
}
