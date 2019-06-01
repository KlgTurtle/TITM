#pragma once
#include "TLS.h"
#include "ITLSExtension.h"

struct ServerSupportedVersions : public ITLSExtension
{
	ServerSupportedVersions(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::supported_versions; }

	ProtocolVersion ChosenVersion;
};