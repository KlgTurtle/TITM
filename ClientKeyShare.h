#pragma once
#include "ITLSExtension.h"
#include "NamedGroups.h"
#include <vector>

struct KeyShareEntry  
{
	NamedGroup Group;
	std::vector<unsigned char> KeyExchangeData;
};

struct ClientKeyShare : public ITLSExtension
{
	ClientKeyShare(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::key_share; }

	std::vector<KeyShareEntry> KeyShareList;
};