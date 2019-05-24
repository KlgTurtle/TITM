#pragma once
#include "ITLSExtension.h"
#include "NamedGroups.h"
#include <vector>

struct Padding : public ITLSExtension
{
	Padding(const std::vector<char>& Buffer, size_t& Offset, unsigned short _PaddingSize, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::padding; }

	std::vector<unsigned char> PaddingData;
	unsigned short PaddingSize = 0;
};