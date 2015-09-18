#include "stdafx.h"
#include "bitObject.h"

bitObject::bitObject(int type, std::string data)
{
	this->type = type;
	this->data = data;
	generateValue();
}

int bitObject::isValidPacket(std::string data)
{
	//int = error code
	//0 = IS FINE
	//1 = repeating 1 symbols
	//2 = not divisible by 8
	//3 = length > 8000
	std::string packet = data;//.substr(8, data.length() - 16);

	//std::cout << "Packet is:" << packet << std::endl;

	std::string finalPacket;
	std::stringstream ss;
	short counter = 0;

	for (int i = 0; i < packet.length(); i++)
	{
		if (counter == 5)
		{
			if (packet.at(i) == '1')
			{
				//std::cout << "Uncorrect packet: " << packet << "\nIDX: " << i << ", " << packet.at(i) << std::endl;
				return 1;
			}
			counter = 0;
		}
		else
		{
			if (packet.at(i) == '1')
			{
				counter += 1;
			}
			else counter = 0;
			finalPacket += packet.at(i);
		}
	}

	if ((float)finalPacket.length() / 8 != (int)finalPacket.length() / 8)
	{
		//std::cout << "Packet is bad (isnt divisible by 8), length is " << finalPacket.length() << std::endl;
		return 2;
	}
	if (finalPacket.length() > 8000)
	{
		//std::cout << "Packet is too big" << std::endl;
		return 3;
	}

	return 0;
}

void bitObject::createType1Value()
{
	value = "01";
	value += converter::decimalToHex(data.length());
}

void bitObject::createType2Value()
{
	value = "02";
	value += converter::decimalToHex(data.length()/8);
}

void bitObject::createType3Value()
{
	value = "03";
	//value += converter::decimalToHex(data.length());

	std::string packet = data.substr(8, data.length() - 16);
	std::string finalPacket;
	std::stringstream ss;
	short counter = 0;

	for (int i = 0; i < packet.length(); i++)
	{
		if (counter == 5)
		{
			if (packet.at(i) == '1')
			{
				std::cout << "Uncorrect packet: " << packet << "\nIDX: " << i << ", " << packet.at(i) << std::endl;
				return;
			}
			counter = 0;
		}
		else
		{
			if (packet.at(i) == '1')
			{
				counter += 1;
			}
			else counter = 0;
			finalPacket += packet.at(i);
		}
	}

	//value = finalPacket;

	value += converter::decimalToHex(finalPacket.length() / 8);

	for (int i = 0; i < finalPacket.length(); i += 4)
	{
		value += converter::binaryToHex(finalPacket.substr(i, 4));
	}

	//std::cout << "the packet is: " << finalPacket << std::endl;
	//value = "Error";
}

void bitObject::createType4Value()
{
	value = "04";
	value += converter::decimalToHex(data.length());
}

void bitObject::generateValue()
{
	switch (type)
	{
		case 1:
		{
			createType1Value();
			break;
		}
		case 2:
		{
			createType2Value();
			break;
		}
		case 3:
		{
			createType3Value();
			break;
		}
		case 4:
		{
			createType4Value();
			break;
		}
		default:
		{
			std::cout << "Wrong type, cant generate value" << std::endl;
		}
	}
}

std::string bitObject::getValue()
{
	return value;
}

bitObject::~bitObject()
{
}
