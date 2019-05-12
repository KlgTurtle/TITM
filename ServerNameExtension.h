#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>

struct ServerNameExtension : public ITLSExtension
{
	ServerNameExtension(const std::vector<char>& Buffer, size_t& Offset);
	std::vector<std::string> ServerNamesList;
};