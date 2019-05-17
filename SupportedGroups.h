#pragma once
#include "ITLSExtension.h"
#include <string>
#include <vector>

enum class NamedCurve : unsigned short
{
	//deprecated(1..22),
	secp256r1 = 23,
	secp384r1 = 24,
	secp521r1 = 25,
	x25519 = 29,
	x448 = 30
	//reserved(0xFE00..0xFEFF),
	//deprecated(0xFF01..0xFF02),	
};

//typedef std::underlying_type<NamedCurve>::type _NamedCurve;


struct SupportedGroups : public ITLSExtension
{
	SupportedGroups(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);

	std::vector<NamedCurve> SupportedGroupsList;
};