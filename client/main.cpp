#include "TaskFactory.h"
#include <iostream>

int main(int argc, const char* argv[])
{
	try
	{
		TaskFactory::Run(argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Standard exception: " << e.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cerr << "Unknown exception occurred." << std::endl;
		return -1;
	}
	return 0;
}
