#include "CommandFactory.h"
#include "ProgramOptions.h"
#include "Config.h"

const SCommand CommandFactory::Create(int argc, const char* argv[])
{
	return ProgramOptions::Create(argc, argv);
}
