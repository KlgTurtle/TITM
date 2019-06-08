#include "ServerCertificate.h"
#include <algorithm>

ServerCertificate::ServerCertificate(const std::vector<char>& Buffer, size_t & Offset)
{
	Deserialize(Buffer, Offset);
}

std::string ServerCertificate::ToString()
{
	return std::string();
}

void ServerCertificate::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	DeserializePlaintextHeader(Buffer, Offset);
	DeserializeHandshakeHeader(Buffer, Offset);

	this->Certificates.clear();

	unsigned int AllCertsLength = SerializationHelper::Deserialize3BytesInt(Buffer, Offset);
	size_t CertsEndOffset = Offset + AllCertsLength;

	while (Offset < CertsEndOffset)
	{
		unsigned int CurrCertLength = SerializationHelper::Deserialize3BytesInt(Buffer, Offset);
	
		std::vector<char> CurrCert;
		SerializationHelper::DeserializeFixedVec<char>(Buffer, Offset, CurrCert, CurrCertLength);

		this->Certificates.push_back(CurrCert);
	}
}

void ServerCertificate::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	unsigned int AllCertsLength = 0;

	std::for_each(this->Certificates.begin(), this->Certificates.end(),
		[&AllCertsLength](const std::vector<char>& v) { AllCertsLength += v.size(); });

	SerializationHelper::Serialize3BytesInt(AllCertsLength, Buffer, Offset);

	for each (const std::vector<char>& CurrCert in this->Certificates)
	{
		unsigned int CurrCertLength = CurrCert.size();

		SerializationHelper::Serialize3BytesInt(CurrCertLength, Buffer, Offset);
		SerializationHelper::SerializeFixedVec<char>(CurrCert, CurrCert.size(), Buffer, Offset);
	}
}
