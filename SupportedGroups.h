#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>

struct SupportedGroups : public ITLSExtension
{
	SupportedGroups(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);

	std::vector<unsigned short> SupportedGroupsList;
};