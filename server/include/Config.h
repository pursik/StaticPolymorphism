#pragma once

#include <cstdint> 

struct SCommand
{
	std::uint64_t port{};///< Port name for network communication.
	bool crypto{ false };///< Flag indicating whether encryption is enabled.
};
