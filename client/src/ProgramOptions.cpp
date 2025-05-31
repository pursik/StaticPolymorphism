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
	constexpr auto SOURCE_CMD = "source";
	constexpr auto SOURCE_OPTION = "source,s";
	constexpr auto ServerName_CMD = "server_name";
	constexpr auto ServerName_OPTION = "server_name";
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
		(SOURCE_OPTION, po::value<std::vector<std::string>>(), "Source file name(s).")
		(ServerName_OPTION, po::value<std::string>()->default_value("localhost"), "Server name.")
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

		if (argc == 1 || commands_.count("help") > 0)
		{
			std::cout << description_ << std::endl;
			return {};
		}

		const auto sources = commands_.count(SOURCE_CMD)
			? std::optional<std::vector<std::string>>(commands_[SOURCE_CMD].as<std::vector<std::string>>())
			: std::nullopt;
		const auto serverName = commands_[ServerName_CMD].as<std::string>();
		const auto port = commands_[PortName_CMD].as<std::uint64_t>();
		const auto crypto = commands_[Crypto_CMD].as<bool>();

		return { sources, serverName, port, crypto };
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

