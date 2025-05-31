#pragma once

#include "ProgramOptions.h"
#include "Config.h"
#include <string>
#include <iostream>
#include <vector>

namespace
{
	namespace po = boost::program_options;

	// Command constants
	constexpr auto PortName_CMD = "port";
	constexpr auto PortName_OPTION = "port";
	constexpr auto Crypto_CMD = "crypto";
	constexpr auto Crypto_OPTION = "crypto";
}

ProgramOptions::ProgramOptions()
	: description_("Allowed options")
{
	description_.add_options()
		("help,h", "Display help message.")
		(PortName_OPTION, po::value<std::uint64_t>()->default_value(12345), "Port number.")
		(Crypto_OPTION, po::value<bool>()->default_value(false), "Encryption.");
}

const SCommand ProgramOptions::Parse(int argc, const char* argv[])
{
	try
	{
		po::store(
			po::command_line_parser(argc, argv)
			.options(description_)
			.style(po::command_line_style::unix_style | po::command_line_style::case_insensitive)
			.run(),
			commands_);
		po::notify(commands_);

		if (commands_.count("help") > 0)
		{
			std::cout << description_ << std::endl;
			return {};
		}
		const auto port = commands_[PortName_CMD].as<std::uint64_t>();
		const auto crypto = commands_[Crypto_CMD].as<bool>();
		std::cout<< "Port: " << port << "\n"
				  << "Crypto: " << (crypto ? "Enabled" : "Disabled") << std::endl;
		return { port, crypto };
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		std::cout << description_ << std::endl;
		return {};
	}
}

const SCommand ProgramOptions::Create(int argc, const char* argv[])
{
	return std::shared_ptr<ProgramOptions>(new ProgramOptions)->Parse(argc, argv);
}
