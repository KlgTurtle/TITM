#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>

enum class ECPointFormat : unsigned char 
{
	uncompressed = 0
	//deprecated(1..2),
	//reserved(248..255)
};

struct ECPointFormats : public ITLSExtension
{
	ECPointFormats(const std::vector<char>& Buffer, size_t& Offset, bool bIsEmpty);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual ExtensionType GetType() { return ExtensionType::ec_points_format; }

	std::vector<ECPointFormat> ECPointFormatList;
};