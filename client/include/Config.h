#pragma once
#include <string>
#include <vector>
#include <optional>

/**
 * @struct SCommand
 * @brief Represents the configuration for executing commands.
 *
 * This structure holds the necessary information for executing commands
 * in different modes of operation, such as multi-threading, multi-processing,
 * or network-based execution.
 */
struct SCommand
{
	std::optional < std::vector<std::string>> sources = std::nullopt; ///< List of source file names to process.
	std::string serverName{};///< Server name or IP address for network communication.
	std::uint64_t port{};///< Port name for network communication.
	bool crypto{ false };///< Flag indicating whether encryption is enabled.
};


