#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>
#include "NamedGroups.h"

struct SupportedGroups : public ITLSExtension
{
	SupportedGroups(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::supported_groups; }

	std::vector<NamedGroup> SupportedGroupsList;
};