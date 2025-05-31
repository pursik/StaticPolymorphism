#pragma once

#include "BaseTemplate.h"
#include "Buffer.h"
#include "Const.h"

template <typename Base>
class Reader : public BaseTemplate<Reader<Base>>
{
public:
	Reader(Buffer& buffer) : baseProcessor(buffer)
	{}

	void ProcessData(std::span<const char> data) 
	{
		if (data.size() < consts::headerSize)
			return;

		std::size_t dataoffset{0};//skip id
		while (dataoffset + consts::IDBytes <= data.size())
		{
			data = data.subspan(dataoffset);

			uint32_t dataSize;
			std::memcpy(&dataSize, data.data() + consts::IDBytes + consts::TypeBytes, consts::DataSizeBytes);

			if (dataoffset + dataSize > data.size())
				throw std::invalid_argument("Error: Data size exceeds the available data after offset.");

			baseProcessor.ProcessData(data);
			dataoffset += consts::headerSize + dataSize;
		}
	}

	bool NotifyComplete()
	{
		std::cout << "Reader::NotifyComplete: " << baseProcessor.NotifyComplete() << std::endl;
		while (!baseProcessor.NotifyComplete())
		{
			ProcessData(baseProcessor.WaitNextData());
		}
		return true;
	}
	std::span<const char> WaitNextData()
	{
		return baseProcessor.WaitNextData();
	}

private:
	Base baseProcessor;
};