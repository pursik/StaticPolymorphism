#pragma once

#include <boost/program_options.hpp>

struct SCommand; ///< Forward declaration of SCommand structure.
class ProgramOptions
{
	boost::program_options::options_description description_; ///< Description of the command line options.
	boost::program_options::variables_map commands_; ///< Map of parsed command line options.
	ProgramOptions();

public:
	static const SCommand Create(int argc, const char* argv[]);
	const SCommand Parse(int argc, const char* argv[]);
};

