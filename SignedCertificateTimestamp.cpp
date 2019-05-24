#include "SignedCertificateTimestamp.h"

SignedCertificateTimestamp::SignedCertificateTimestamp(const std::vector<char>& Buffer, size_t & Offset, 
	unsigned short _ExtLength, bool bIsEmpty) : ExtensionLength(_ExtLength)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void SignedCertificateTimestamp::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeFixedVec<char>(OpaqueSCT, OpaqueSCT.size(), Buffer, Offset);
}

void SignedCertificateTimestamp::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::DeserializeFixedVec<char>(Buffer, Offset, OpaqueSCT, ExtensionLength);
}
