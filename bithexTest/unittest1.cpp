
#include "stdafx.h"
#include "CppUnitTest.h"

#include "parser.h"
#include <iostream>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace bithexTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(preparationSpeedTest)
		{
			std::string testString;

			for (int i = 0; i < 65536; i++)
			{
				testString += "FFFF";
			}
			
			parser p;
			p.prepareString(testString, 65536 * 8);
		}

		TEST_METHOD(preparationTest)
		{
			std::string s = ":):):):)";
			parser p;
			Assert::AreEqual(p.prepareString(s, 300), false);

			s = "AAA";
			Assert::AreEqual(p.prepareString(s, 300), false);

			s = "0111111001111110";
			Assert::AreEqual(p.prepareString(s, 10), false);

			s = "";
			Assert::AreEqual(p.prepareString(s, 10), false);
		}

		TEST_METHOD(parsingSpeedTest)
		{
			std::string testString;

			for (int i = 0; i < 65536; i++)
			{
				testString += "FFFF";
			}

			parser p;
			p.parseString(testString, 65536 * 8);
		}

		TEST_METHOD(parsingTest)
		{
			std::string s = "01000D04000C0200010300040377EA9C01000E";
			parser p;
			p.parseString("FF5FA5FCFC80DD57E5F8FDFF", 300);
			Assert::AreEqual(p.getParsed(), s);

			s = "020001";
			p.parseString("7E", 300);
			Assert::AreEqual(p.getParsed(), s);

			s = "010008";
			p.parseString("FF", 300);
			Assert::AreEqual(p.getParsed(), s);

			s = "040008";
			p.parseString("A1", 300);
			Assert::AreEqual(p.getParsed(), s);

			s = "";
			p.parseString("7EA1", 300);
			Assert::AreEqual(p.getParsed(), s);
			
			p = parser();
			s = "010008020001";
			p.parseString("FF7E", 300);
			Assert::AreEqual(p.getParsed(), s);

			s = "020001010008";
			p.parseString("7EFF", 300);
			Assert::AreEqual(p.getParsed(), s);
		}

		TEST_METHOD(packetValidation)
		{
			//repeatable 1
			Assert::AreEqual(bitObject::isValidPacket("111111111111"), 1);

			//isnt divisible by 8
			Assert::AreEqual(bitObject::isValidPacket("110110110"), 2);
			
			//still isnt divisible by 8 after shrinking
			Assert::AreEqual(bitObject::isValidPacket("00111001010101011111011101100000"), 2);

			//length is over 8000
			std::string s;
			for (int i = 0; i < 8008; i++)
			{
				s += "0";
			}
			Assert::AreEqual(bitObject::isValidPacket(s), 3);
		}

		TEST_METHOD(converterHexToBinary)
		{
			std::string s = "1111";
			Assert::AreEqual(converter::hexToBinary('F'), s);

			s = "0000";
			Assert::AreEqual(converter::hexToBinary('0'), s);

			s = "INCORRECT HEX VALUE";
			Assert::AreEqual(converter::hexToBinary('G'), s);
		}

		TEST_METHOD(converterHexToReversedBinary)
		{
			std::string hex = "0123456789ABCDEF";
			
			for (int i = 0; i < hex.length(); i++)
			{
				std::string s = converter::hexToBinary(hex[i]);
				std::reverse(s.begin(), s.end());

				Assert::AreEqual(converter::hexToReversedBinary(hex[i]), s);
			}			
		}

		TEST_METHOD(converterDecToHex)
		{
			std::string s = "FFFF";
			Assert::AreEqual(converter::decimalToHex(65535), s);

			s = "0000";
			Assert::AreEqual(converter::decimalToHex(0), s);
		}

		TEST_METHOD(converterBinaryToHex)
		{
			std::string s = "0";
			Assert::AreEqual(converter::binaryToHex("0000"), s);

			s = "INCORRECT DATA LENGTH (NOT EQUAL 4)";
			Assert::AreEqual(converter::binaryToHex("191023"), s);

			s = "INCORRECT BINARY VALUE";

			//Logger::WriteMessage(converter::binaryToHex("2222").c_str());
			Assert::AreEqual(converter::binaryToHex("2222"), s);		
		}

		TEST_METHOD(bitObjects)
		{
			std::string s = "01000D";
			bitObject b = bitObject(1, "1111111111111");
			Assert::AreEqual(b.getValue(), s);

			s = "04000C";
			b = bitObject(4, "010100101010");
			Assert::AreEqual(b.getValue(), s);

			s = "020002";
			b = bitObject(2, "0111111001111110");
			Assert::AreEqual(b.getValue(), s);

			s = "0300043955FEC0";
			b = bitObject(3, "0111111000111001010101011111011101100000001111110");
			Assert::AreEqual(b.getValue(), s);
		}
	};
}