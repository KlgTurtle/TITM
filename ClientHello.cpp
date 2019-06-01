#include <WinSock2.h>
#include "TLS.h"
#include "ClientHello.h"


ClientHello::ClientHello(const std::vector<char>& Buffer, size_t& Offset)
{
	Deserialize(Buffer, Offset);
}

std::string ClientHello::ToString()
{
	std::string TLSHeader = ITLSMessage::ToString();

	return "";
}

void ClientHello::Deserialize(const std::vector<char>& Buffer, size_t& Offset)
{
	DeserializePlaintextHeader(Buffer, Offset);
	DeserializeHandshakeHeader(Buffer, Offset);

	SerializationHelper::DeserializeStruct(Buffer, Offset, reinterpret_cast<char*>(&CHHeader), sizeof(CHHeader));
	SerializationHelper::DeserializeVec<unsigned char, unsigned char>(Buffer, Offset, this->session_id);
	SerializationHelper::DeserializeVec<unsigned short, unsigned short>(Buffer, Offset, this->cipher_suites);
	SerializationHelper::DeserializeVec<unsigned char, unsigned char>(Buffer, Offset, this->compression_methods);

	GetExtensions(Buffer, Offset, this->extensions);
}

void ClientHello::Serialize(std::vector<char>& Buffer, size_t& Offset)
{
	std::vector<char> DummyBuffer;
	DummyBuffer.clear();

	// Don't serialize headers now, because we may need to update the lengths
	SerializePlaintextHeader(DummyBuffer, Offset);
	SerializeHandshakeHeader(DummyBuffer, Offset);

	SerializationHelper::SerializeStruct(reinterpret_cast<char*>(&CHHeader), sizeof(CHHeader), Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned char, unsigned char>(this->session_id, Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned short, unsigned short>(this->cipher_suites, Buffer, Offset);
	SerializationHelper::SerializeVec<unsigned char, unsigned char>(this->compression_methods, Buffer, Offset);

	// Leave space for total extensions length - we'll know it later.
	size_t ExtStartOffset = Offset;
	Offset += sizeof(unsigned short);

	for (size_t i = 0; i < extensions.size(); ++i)
	{
		SerializationHelper::Serialize<ExtensionType>(extensions[i]->GetType(), Buffer, Offset);
		size_t ExtLenOffset = Offset;

		// Leave space for this specific extension length
		Offset += sizeof(unsigned short);

		extensions[i]->Serialize(Buffer, Offset);

		// Update the two bytes we left earlier with the total extension length
		unsigned short ExtensionLength = Offset - ExtLenOffset - sizeof(unsigned short);
		SerializationHelper::Serialize<unsigned short>(ExtensionLength, Buffer, ExtLenOffset);
	}

	// Update the two bytes we left (much) earlier with the total length of all extensions
	unsigned short TotalExtensionsLength = Offset - ExtStartOffset - sizeof(unsigned short);
	SerializationHelper::Serialize<unsigned short>(TotalExtensionsLength, Buffer, ExtStartOffset);

	// Now that we know the actual total length, we can update the TLS record header and
	// the Handshake header with the appropriate ones and serialize them.
	this->TLSHeader.length = Offset - sizeof(TLSPlaintextHeader);
	this->HSHeader.length = this->TLSHeader.length - sizeof(HandshakeHeaderSerialized);
	size_t OffsetTemp = 0;
	SerializePlaintextHeader(Buffer, OffsetTemp);
	SerializeHandshakeHeader(Buffer, OffsetTemp);
}
