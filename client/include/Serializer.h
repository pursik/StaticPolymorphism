#pragma once

#include <span>
#include <vector>
#include <optional>
#include "BaseTemplate.h"
#include "Const.h"

class Serializer : public BaseTemplate<Serializer>
{
public:
	void ProcessData(std::span<const char> data)
	{
		buffer.clear();
		auto dataSize = static_cast<uint32_t>(data.size());
		auto dataSizePtr = reinterpret_cast<char*>(&dataSize);
		buffer.insert(buffer.end(), dataSizePtr, std::next(dataSizePtr, sizeof(uint32_t)));
		buffer.insert(buffer.end(), data.begin(), data.end());
	}

	bool NotifyComplete()
	{
		return !buffer.empty();
	}

	std::span<const char> WaitNextData()
	{
		return { buffer };
	}

private:
	std::vector<char> buffer;
	consts::DataType type_{ consts::DataType::Type_File_Name };
};