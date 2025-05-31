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
class Encryptor : public BaseTemplate<Encryptor<Base>>
{
	inline static const CryptoPP::SecByteBlock key{ reinterpret_cast<const CryptoPP::byte*>(consts::InitKey.data()), consts::InitKey.size() };
	inline static const CryptoPP::SecByteBlock iv{ reinterpret_cast<const CryptoPP::byte*>(consts::InitIV.data()), consts::InitIV.size() };

	Base baseProcessor;
public:
	Encryptor() = default;
	Encryptor(Buffer& buffer) :baseProcessor(buffer) {};

	void ProcessData(std::span<const char> data)
	{
		std::cout <<std::endl;
		CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryptor;
		encryptor.SetKeyWithIV(key, key.size(), iv.data());

		std::vector<char> result;
		result.resize(data.size());
		encryptor.ProcessData(
			reinterpret_cast<CryptoPP::byte*>(result.data()),
			reinterpret_cast<const CryptoPP::byte*>(data.data() ),
			data.size()
		);
		std::cout << "Encryption successful!" << std::endl;
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
