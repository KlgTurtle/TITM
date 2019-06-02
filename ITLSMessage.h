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

struct ITLSHandshakeMessage : public ITLSMessage
{
	HandshakeHeader HSHeader;

	virtual HandshakeType GetHandshakeType() = 0;
	virtual ContentType GetType() { return ContentType::handshake; }
	virtual void SerializeHandshakeHeader(std::vector<char>& Buffer, size_t& Offset);
	virtual void DeserializeHandshakeHeader(const std::vector<char>& Buffer, size_t& Offset);
	virtual void GetExtensions(const std::vector<char>& Buffer, size_t& Offset, std::vector<std::shared_ptr<ITLSExtension>>& outExtensions);
	virtual void SerializeExtensions(const std::vector<std::shared_ptr<ITLSExtension>>& inExtensions, std::vector<char>& Buffer, size_t & Offset);
};