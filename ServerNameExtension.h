#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>

struct ServerNameExtension : public ITLSExtension
{
	ServerNameExtension(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::server_name; }

	std::vector<std::string> ServerNamesList;
};