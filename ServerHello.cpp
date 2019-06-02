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
	std::vector<char> DummyBuffer;
	DummyBuffer.clear();

	// Don't serialize headers now, because we may need to update the lengths
	SerializePlaintextHeader(DummyBuffer, Offset);
	SerializeHandshakeHeader(DummyBuffer, Offset);

	SerializationHelper::SerializeStruct(reinterpret_cast<char*>(&this->SHHeader), sizeof(this->SHHeader), Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned char, unsigned char>(this->session_id, Buffer, Offset);
	SerializationHelper::Serialize<unsigned short>(this->cipher_suite, Buffer, Offset);
	SerializationHelper::Serialize<unsigned char>(this->legacy_compression_method, Buffer, Offset);

	SerializeExtensions(extensions, Buffer, Offset);

	// Now that we know the actual total length, we can update the TLS record header and
	// the Handshake header with the appropriate ones and serialize them.
	this->TLSHeader.length = Offset - sizeof(TLSPlaintextHeader);
	this->HSHeader.length = this->TLSHeader.length - sizeof(HandshakeHeaderSerialized);
	size_t OffsetTemp = 0;
	SerializePlaintextHeader(Buffer, OffsetTemp);
	SerializeHandshakeHeader(Buffer, OffsetTemp);

}


