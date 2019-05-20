#pragma once
#include "ITLSExtension.h"
#include "NamedGroups.h"
#include <vector>

struct Padding : public ITLSExtension
{
	Padding(const std::vector<char>& Buffer, size_t& Offset, unsigned short _PaddingSize);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);

	std::vector<unsigned char> PaddingData;
	unsigned short PaddingSize = 0;
};