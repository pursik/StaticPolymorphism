#pragma once
#include <string_view>
namespace consts
{
	constexpr auto DefaultChunkSize = 4096U;
	constexpr auto DefaultChunkAmountInBuffer = 128U;
	constexpr auto BufferSize = DefaultChunkSize * DefaultChunkAmountInBuffer;

	constexpr std::string_view InitKey = "7D9BB722DA2DC8674E08C3D44AAE976F";
	constexpr std::string_view InitIV = "37C6D22FADE22B2D924598BEE2455EFC";

	constexpr auto IDBytes = 1U;
	constexpr auto TypeBytes = 1U;
	constexpr auto DataSizeBytes = 4U;
	constexpr auto headerSize = consts::IDBytes + consts::TypeBytes + consts::DataSizeBytes;

	enum class DataType : std::uint8_t
	{
		Type_File_Name = 1U,
		Type_File_Data = 2U,
	};
}