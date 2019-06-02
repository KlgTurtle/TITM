#pragma once
#include "ITLSExtension.h"
#include "NamedGroups.h"
#include <vector>

struct ServerPreSharedKey : public ITLSExtension
{
	ServerPreSharedKey(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::pre_shared_key; }

	unsigned short SelectedIdentity;
};