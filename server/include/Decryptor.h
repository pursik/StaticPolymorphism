#pragma once

#include "BaseTemplate.h"
#include "Const.h"
#include "Buffer.h"

#include <cryptopp/modes.h>
#include <cryptopp/aes.h>

#include <iostream>
#include <vector>
#include <string>

template <typename Base>
class Decryptor : public BaseTemplate<Decryptor<Base>>
{
	inline static const CryptoPP::SecByteBlock key{ reinterpret_cast<const CryptoPP::byte*>(consts::InitKey.data()), consts::InitKey.size() };
	inline static const CryptoPP::SecByteBlock iv{ reinterpret_cast<const CryptoPP::byte*>(consts::InitIV.data()), consts::InitIV.size() };

	Base baseProcessor;
public:
	Decryptor() = default;
	Decryptor(Buffer& buffer) :baseProcessor(buffer) {};

	void ProcessData(std::span<const char> data)
	{
		std::cout <<std::endl;
		CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption decryptor;
		decryptor.SetKeyWithIV(key, key.size(), iv.data());

		std::vector<char> result(data.size());
		std::memcpy(result.data(), data.data(), consts::headerSize);
		
		auto payloadSize = data.size() - consts::headerSize; // Skip ID, Type, and Data Size bytes
		decryptor.ProcessData(
			reinterpret_cast<CryptoPP::byte*>(result.data() + consts::headerSize),
			reinterpret_cast<const CryptoPP::byte*>(data.data() + consts::headerSize),
			payloadSize
		);
		
		std::cout << "Decryption successful!" << std::endl;
		baseProcessor.ProcessData({ reinterpret_cast<char*>(result.data()), result.size() });
	}

	bool NotifyComplete()
	{
		return baseProcessor.NotifyComplete();
	}

	std::span<const char> WaitNextData()
	{
		return baseProcessor.WaitNextData();
	}
};
