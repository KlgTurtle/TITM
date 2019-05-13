#pragma once
#include "ITLSExtension.h"
#include <vector>

struct ClientSupportedVersions : public ITLSExtension
{
	ClientSupportedVersions(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);

	std::vector<unsigned short> SupportedVersionsList;
};