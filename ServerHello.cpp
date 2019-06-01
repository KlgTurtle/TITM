#include "ServerHello.h"

ServerHello::ServerHello(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

std::string ServerHello::ToString()
{
	return "";
}

void ServerHello::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	DeserializePlaintextHeader(Buffer, Offset);
	DeserializeHandshakeHeader(Buffer, Offset);

	SerializationHelper::DeserializeStruct(Buffer, Offset, reinterpret_cast<char*>(&this->SHHeader), sizeof(this->SHHeader));
	SerializationHelper::DeserializeVec<unsigned char, unsigned char>(Buffer, Offset, this->session_id);
	this->cipher_suite = SerializationHelper::Deserialize<unsigned short>(Buffer, Offset);
	this->legacy_compression_method = SerializationHelper::Deserialize<unsigned char>(Buffer, Offset);

	GetExtensions(Buffer, Offset, this->extensions);
}

void ServerHello::Serialize(std::vector<char>& Buffer, size_t & Offset)
{

}


