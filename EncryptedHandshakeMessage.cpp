#include "EncryptedHandshakeMessage.h"

EncryptedHandshakeMessage::EncryptedHandshakeMessage(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

std::string EncryptedHandshakeMessage::ToString()
{
	return std::string();
}

void EncryptedHandshakeMessage::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	DeserializePlaintextHeader(Buffer, Offset);
	SerializationHelper::DeserializeFixedVec<char>(Buffer, Offset, this->EncryptedData, this->TLSHeader.length);
}

void EncryptedHandshakeMessage::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializePlaintextHeader(Buffer, Offset);
	SerializationHelper::SerializeFixedVec<char>(this->EncryptedData, this->EncryptedData.size(), Buffer, Offset);
}
