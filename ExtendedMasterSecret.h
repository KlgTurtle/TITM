#pragma once
#include "ITLSExtension.h"
#include <vector>


struct ExtendedMasterSecret : public ITLSExtension
{
	ExtendedMasterSecret(const std::vector<char>& Buffer, size_t& Offset, unsigned short _ExtLength, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::extended_master_secret; }

	std::vector<char> Data; // legally should be empty for this extension (rfc7627)
	size_t ExtLength;
}; 