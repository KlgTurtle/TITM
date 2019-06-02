#pragma once
#include "ITLSMessage.h"

struct ChangeCipherSpec : public ITLSMessage
{
public:
	ChangeCipherSpec(const std::vector<char>& Buffer, size_t& Offset);

	virtual std::string ToString();

	virtual ContentType GetType() { return ContentType::change_cipher_spec; }

	unsigned char ChangeCipherSpecMsg;

protected:
	virtual void Deserialize(const std::vector<char>& Buffer, size_t& Offset);
	virtual void Serialize(std::vector<char>& Buffer, size_t& Offset);

};