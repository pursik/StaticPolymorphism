#pragma once
#include "BaseTemplate.h"
#include "Buffer.h"
#include "Const.h"
#include <iostream>
#include <vector>

template <typename Base>
class Serializer : public BaseTemplate<Serializer<Base>>
{
public:
	Serializer(Buffer& buffer) : baseProcessor(buffer)
	{}

	void ProcessData(std::span<const char> data)
	{
		const auto dataType = data[1];
		if (dataType != static_cast<char>(consts::DataType::Type_File_Data) &&
			dataType != static_cast<char>(consts::DataType::Type_File_Name))
		{
			throw std::invalid_argument("Error: Invalid data type received.");
		}
		std::vector<char> buffer;
		buffer.reserve(consts::IDBytes + consts::TypeBytes + data.size());
		buffer.push_back(data[0]); // id
		buffer.push_back(dataType); // type
		data = data.subspan(consts::headerSize); // Skip ID and Type bytes and size the data
		buffer.insert(buffer.end(), data.begin(), data.end());
		baseProcessor.ProcessData(buffer);
	}

	bool NotifyComplete()
	{
		return baseProcessor.NotifyComplete();
	}

	std::span<const char> WaitNextData()
	{
		return baseProcessor.WaitNextData();
	}
private:
	Base baseProcessor;
};