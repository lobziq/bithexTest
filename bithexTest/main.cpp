#include <iostream>
#include <string>
#include <map>

#include <climits>

#include "converter.h"
#include "bitObject.h"
#include "parser.h"

int main()
{
	const int maxLength = 600; //300 bytes is 600 hex symbols as one stands for 0.5 byte
	std::string s;
	parser p;

	while (std::getline(std::cin, s))
	{
		p.parseString(s, maxLength);

		std::cout << p.getParsed() << std::endl;
	}

	p.transferIsOver();
	p.parseString("", maxLength);
	std::cout << p.getParsed() << std::endl;

	std::getchar();

	return 0;
}