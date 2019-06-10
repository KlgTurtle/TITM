#pragma once
#include "ITLSMessage.h"

struct EncryptedHandshakeMessage : public ITLSMessage
{
public:
	EncryptedHandshakeMessage(const std::vector<char>& Buffer, size_t& Offset);

	virtual std::string ToString();

	virtual ContentType GetType() { return ContentType::handshake; }

	std::vector<char> EncryptedData;

	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);

};