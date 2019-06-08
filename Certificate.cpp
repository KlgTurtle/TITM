#include "Certificate.h"

Certificate::Certificate(const std::vector<char>& Buffer, size_t & Offset, unsigned int _HSHeaderLength)
	: HSHeaderLength(_HSHeaderLength)
{
	Deserialize(Buffer, Offset);
}

std::string Certificate::ToString()
{
	return std::string();
}

void Certificate::DetermineStructure(const std::vector<char>& Buffer, size_t Offset, bool& GotReqContext, bool& GotExtensions)
{
	

	unsigned int PossibleLenReqContext = SerializationHelper::Deserialize<unsigned char>(Buffer, Offset);
	std::vector<unsigned char> CertLengthBuf;
	CertLengthBuf.resize(sizeof(unsigned int));
	memset(CertLengthBuf.data(), 0x00, CertLengthBuf.size());
	memcpy(&CertLengthBuf[1], &Buffer[Offset], sizeof(unsigned int) - 1);

	unsigned int PossibleCertLen = ntohl(*reinterpret_cast<unsigned long*>(CertLengthBuf.data()));

	// Option 1: we only have certificates, return and indicate there is nothing else in this message.
	if (PossibleCertLen + sizeof(unsigned int) - 1 == HSHeaderLength)
	{
		GotReqContext = GotExtensions = false;
		return;
	}
	// Option 2: The length of the certificates part doesn't cover the entire message, there's something else here...
	else
	{
		
	}
	
}

void Certificate::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	DeserializePlaintextHeader(Buffer, Offset);
	DeserializeHandshakeHeader(Buffer, Offset);


	unsigned int possibleLen = SerializationHelper::Deserialize<unsigned char>(Buffer, Offset);
	if (Offset + possibleLen < HSHeaderLength)
	{
		std::vector<unsigned char> CertLengthBuf;
		CertLengthBuf.resize(sizeof(unsigned int));
		memset(CertLengthBuf.data(), 0x00, CertLengthBuf.size());
		memcpy(&CertLengthBuf[1], &Buffer[Offset], sizeof(unsigned int) - 1);

		unsigned int CertLen = ntohl(*reinterpret_cast<unsigned long*>(CertLengthBuf.data()));
	}

	std::vector<unsigned char> CertLengthBuf;
	CertLengthBuf.resize(sizeof(unsigned int));
	memset(CertLengthBuf.data(), 0x00, CertLengthBuf.size());
	memcpy(&CertLengthBuf[1], &Buffer[Offset], sizeof(unsigned int) - 1);

	unsigned int CertLen = ntohl(*reinterpret_cast<unsigned long*>(CertLengthBuf.data()));

	Offset += sizeof(unsigned int) - 1;

	SerializationHelper::DeserializeVec<char, unsigned int>(Buffer, Offset, this->cert_data);


}

void Certificate::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
}
