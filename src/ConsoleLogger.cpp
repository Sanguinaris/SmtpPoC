#include "ConsoleLogger.h"

#include <iostream>

void ConsoleLogger::Log(const std::string & str)
{
	std::cout << str << '\n';
}
