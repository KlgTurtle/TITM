#pragma once
#include "ITLSExtension.h"
#include "NamedGroups.h"
#include <vector>
#include "ClientKeyShare.h"

struct ServerKeyShare : public ITLSExtension
{
	ServerKeyShare(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::key_share; }

	KeyShareEntry SelectedKeyShare;
};