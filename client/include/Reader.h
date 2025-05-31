#pragma once

#include "BaseTemplate.h"
#include <vector>
#include <fstream>
#include <span>

template <typename Base>
class Reader : public BaseTemplate<Reader<Base>>
{
public:
	Reader() = default;

	void ProcessData(std::span<const char> source)
	{
		if (source.empty())
		{
			if (!stream_.read(chunkBuffer.data(), chunkBuffer.size()))
			{
				if (stream_.gcount() == 0U)
				{
					return;
				}
				chunkBuffer.resize(stream_.gcount());/*actually read amount*/
			}
			baseProcessor.ProcessData(chunkBuffer);
		}
		else
		{
			stream_.open(source.data(), std::ios::binary);
			if (!stream_.is_open() || !stream_.good())
			{
				throw std::runtime_error("file is failed to open.");
			}
			chunkBuffer.resize(consts::DefaultChunkSize);
			baseProcessor.ProcessData(source);
		}
	}

	bool NotifyComplete()
	{
		return stream_.eof();
	}

	std::span<const char> WaitNextData()
	{
		return baseProcessor.WaitNextData();
	}
private:
	std::ifstream stream_; ///< Input file stream
	std::vector<char> chunkBuffer; ///< Buffer to store chunks of data
	Base baseProcessor; ///< Base processor for handling data
};