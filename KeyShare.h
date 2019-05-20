#pragma once
#include "ITLSExtension.h"
#include "NamedGroups.h"
#include <vector>

struct KeyShareEntry  
{
	NamedGroup Group;
	std::vector<unsigned char> KeyExchangeData;
};

struct KeyShare : public ITLSExtension
{
	KeyShare(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);

	std::vector<KeyShareEntry> KeyShareList;
};