#include "TLS.h"

void TLSPlaintextHeader::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	this->type = SerializationHelper::Deserialize<ContentType>(Buffer, Offset);
	this->version.major = SerializationHelper::Deserialize<unsigned char>(Buffer, Offset);
	this->version.minor = SerializationHelper::Deserialize<unsigned char>(Buffer, Offset);
	this->length = SerializationHelper::Deserialize<unsigned short>(Buffer, Offset);
}

void TLSPlaintextHeader::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::Serialize<ContentType>(this->type, Buffer, Offset);
	SerializationHelper::Serialize<unsigned char>(this->version.major, Buffer, Offset);
	SerializationHelper::Serialize<unsigned char>(this->version.minor, Buffer, Offset);
	SerializationHelper::Serialize<unsigned short>(this->length, Buffer, Offset);
}

void HandshakeHeader::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	HandshakeHeaderSerialized SerializedHSHeader;
	SerializationHelper::DeserializeStruct(Buffer, Offset, reinterpret_cast<char*>(&SerializedHSHeader),
		sizeof(SerializedHSHeader));

	this->msg_type = SerializedHSHeader.msg_type;

	std::vector<unsigned char> HSHeaderLengthBuf;
	HSHeaderLengthBuf.resize(sizeof(this->length));
	memset(HSHeaderLengthBuf.data(), 0x00, HSHeaderLengthBuf.size());
	memcpy(&HSHeaderLengthBuf[1], SerializedHSHeader.length, sizeof(SerializedHSHeader.length));

	this->length = ntohl(*reinterpret_cast<unsigned long*>(HSHeaderLengthBuf.data()));
}

void HandshakeHeader::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	HandshakeHeaderSerialized SerializedHSHeader;
	SerializedHSHeader.msg_type = this->msg_type;

	unsigned int lengthBE = htonl(this->length);
	unsigned char* pLengthBE = 1 + (unsigned char*)&lengthBE;
	memcpy(&SerializedHSHeader.length, pLengthBE, sizeof(SerializedHSHeader.length));
	
	SerializationHelper::SerializeStruct(reinterpret_cast<char*>(&SerializedHSHeader), 
		sizeof(SerializedHSHeader), Buffer, Offset);
}
