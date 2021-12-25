#include <iostream>

namespace Apricot {

	__declspec(dllexport) void PrintTest()
	{
		std::cout << "Welcome to the Apricot Engine!" << std::endl;
	}

}