#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>

enum class PskKeyExchangeMode : unsigned char
{
	psk_ke = 0, 
	psk_dhe_ke = 1
};


struct PskKeyExchangeModes : public ITLSExtension
{
	PskKeyExchangeModes(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::psk_key_exchange_modes; }

	std::vector<PskKeyExchangeMode> PskKeyExchangeModesList;
};