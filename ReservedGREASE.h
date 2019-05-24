#pragma once
#include "ITLSExtension.h"

struct ReservedGREASE : public ITLSExtension
{
	ReservedGREASE(const std::vector<char>& Buffer, size_t& Offset, ExtensionType _GREASEValue, 
		unsigned short _GREASELength, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return GREASEValue; }

	ExtensionType GREASEValue;
	unsigned short GREASELength;
	std::vector<char> GREASEData;
};