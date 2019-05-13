#pragma once

#include <string>
#include <vector>
#include "TLS.h"

struct ITLSMessage
{
	TLSPlaintextHeader TLSHeader;
	virtual std::string ToString();
	virtual void Deserialize(const std::vector<char>& Buffer) = 0;
	virtual void Serialize(std::vector<char>& Buffer) = 0;
};