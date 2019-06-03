#pragma once

#include <string>
#include <vector>
#include "TLS.h"

struct ITLSMessage
{
	TLSPlaintextHeader TLSHeader;

	virtual std::string ToString();
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset) = 0;
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset) = 0;
	virtual void SerializePlaintextHeader(std::vector<char>& Buffer, size_t& Offset);
	virtual void DeserializePlaintextHeader(const std::vector<char>& Buffer, size_t& Offset);
	virtual ContentType GetType() = 0;
};

