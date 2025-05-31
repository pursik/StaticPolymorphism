#pragma once

#include "BaseTemplate.h"
#include "Buffer.h"
#include <fstream>
#include <unordered_map>
#include "Const.h"
#include <iostream>
#include <filesystem>

template <typename Base>
class FileWriter : public BaseTemplate<FileWriter<Base>>
{
public:
	FileWriter(Buffer& buffer) : baseProcessor(buffer)
	{}
	void ProcessData(std::span<const char> data)
	{
		if (data.size() < consts::TypeBytes + consts::IDBytes)
		{
			return;
		}
		const auto id{ data[0] };
		const auto type{ static_cast<consts::DataType>(data[1]) };
		data = data.subspan(consts::TypeBytes + consts::IDBytes);

		if (type == consts::DataType::Type_File_Name)
		{
			const auto fileNameFullPath = GetFileFullName(data, id);

			stream_[id].open(fileNameFullPath.string(), std::ios::binary);
			if (!stream_[id].is_open())
			{
				throw std::runtime_error(fileNameFullPath.string() + " is failed to open.");
			}
		}
		else
			if (type == consts::DataType::Type_File_Data)
			{
				if (!stream_[id].is_open())
				{
					throw std::runtime_error("FileWriter::ProcessData: Stream for ID " + std::to_string(id) + " is not open.");
				}
				stream_[id].write(data.data(), data.size());
				stream_[id].flush();
			}
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
	std::unordered_map<char, std::ofstream> stream_;
	Base baseProcessor;

	inline static const std::string subfoldername = "client_";

	std::filesystem::path GetFileFullName(std::span<const char> data, char id)
	{
		const auto currentDir = std::filesystem::current_path();
		const auto subFolderPath = currentDir / (subfoldername + std::to_string(id));
		if (!std::filesystem::exists(subFolderPath))
		{
			std::filesystem::create_directory(subFolderPath);
		}
		std::filesystem::path fileNameAsPath(data.begin(), data.end());
		return subFolderPath / fileNameAsPath.filename();
	}
};