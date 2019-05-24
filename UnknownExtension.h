#pragma once
#include "ITLSExtension.h"
#include <vector>


struct UnknownExtension : public ITLSExtension
{
	UnknownExtension(const std::vector<char>& Buffer, size_t& Offset, ExtensionType _ExtType, 
		unsigned short _ExtLength, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtType; }

	ExtensionType ExtType;
	size_t        ExtLength;
	std::vector<char> ExtensionData;
};