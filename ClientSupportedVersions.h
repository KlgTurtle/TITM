#pragma once
#include "ITLSExtension.h"
#include <vector>

struct ClientSupportedVersions : public ITLSExtension
{
	ClientSupportedVersions(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::supported_versions; }

	std::vector<unsigned short> SupportedVersionsList;
};