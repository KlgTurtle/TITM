#pragma once
#include "ITLSExtension.h"
#include "NamedGroups.h"
#include <vector>

struct PskIdentity 
{
	std::vector<char> identity;
	unsigned int obfuscated_ticket_age;
};

struct OfferedPsks 
{
	std::vector<PskIdentity> identities;
	std::vector<char> binders;
} ;

struct ClientPreSharedKey : public ITLSExtension
{
	ClientPreSharedKey(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::pre_shared_key; }

	OfferedPsks offeredPsks;
};